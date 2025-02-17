#include <iostream>

#include "http_server.h"
#include "faiss_index.h"
#include "index_factory.h"
#include "logger.h"
#include "constants.h"

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace vdb {

HttpServer::HttpServer(const std::string& host, int port) : host_(host), port_(port) {
    server_.Post("/search", [this](const httplib::Request& req, httplib::Response& res) {
        SearchHandler(req, res);
    });

    server_.Post("/insert", [this](const httplib::Request& req, httplib::Response& res) {
        InsertHandler(req, res);
    });
}

void HttpServer::Start() {
    server_.listen(host_, port_);
}

bool HttpServer::IsRequestValid(const rapidjson::Document& json_request, CheckType check_type) {
    switch (check_type) {
        case CheckType::SEARCH:
            return json_request.HasMember(REQUEST_VECTORS) &&
                   json_request.HasMember(REQUEST_K) &&
                   (!json_request.HasMember(REQUEST_INDEX_TYPE) || json_request[REQUEST_INDEX_TYPE].IsString());
        case CheckType::INSERT:
            return json_request.HasMember(REQUEST_VECTORS) &&
                   json_request.HasMember(REQUEST_ID) &&
                   (!json_request.HasMember(REQUEST_INDEX_TYPE) || json_request[REQUEST_INDEX_TYPE].IsString());
        default:
            return false;
    }
}

IndexFactory::IndexType HttpServer::GetIndexTypeFromRequest(const rapidjson::Document& json_request) {
    // 获取请求参数中的索引类型
    if (json_request.HasMember(REQUEST_INDEX_TYPE)) {
        std::string index_type_str = json_request[REQUEST_INDEX_TYPE].GetString();
        if (index_type_str == "FLAT") {
            return IndexFactory::IndexType::FLAT;
        }
    }
    return IndexFactory::IndexType::UNKNOWN; 
}

void HttpServer::SearchHandler(const httplib::Request& req, httplib::Response& res) {
    global_logger->debug("Received search request");

    // 解析JSON请求
    rapidjson::Document json_request;
    json_request.Parse(req.body.c_str());

    // 打印用户的输入参数
    global_logger->info("Search request parameters: {}", req.body);

    // 检查JSON文档是否为有效对象
    if (!json_request.IsObject()) {
        global_logger->error("Invalid JSON request");
        res.status = 400;
        SetErrorJsonResponse(res, RESPONSE_RETCODE_ERROR, "Invalid JSON request"); 
        return;
    }

    // 检查请求的合法性
    if (!IsRequestValid(json_request, CheckType::SEARCH)) {
        global_logger->error("Missing vectors or k parameter in the request");
        res.status = 400;
        SetErrorJsonResponse(res, RESPONSE_RETCODE_ERROR, "Missing vectors or k parameter in the request"); 
        return;
    }

    // 获取查询参数
    std::vector<float> query;
    for (const auto& q : json_request[REQUEST_VECTORS].GetArray()) {
        query.push_back(q.GetFloat());
    }
    int k = json_request[REQUEST_K].GetInt();

    global_logger->debug("Query parameters: k = {}", k);

    // 获取请求参数中的索引类型
    IndexFactory::IndexType index_type = GetIndexTypeFromRequest(json_request);

    // 如果索引类型为UNKNOWN，返回400错误
    if (index_type == IndexFactory::IndexType::UNKNOWN) {
        global_logger->error("Invalid indexType parameter in the request");
        res.status = 400;
        SetErrorJsonResponse(res, RESPONSE_RETCODE_ERROR, "Invalid indexType parameter in the request"); 
        return;
    }

    // 使用全局IndexFactory获取索引对象
    void* index = GetGlobalIndexFactory()->GetIndex(index_type);

    // 根据索引类型初始化索引对象并调用search_vectors函数
    SearchResult results; // 直接声明results变量
    switch (index_type) {
        case IndexFactory::IndexType::FLAT: {
            FaissIndex* faiss_index = static_cast<FaissIndex*>(index);
            results = faiss_index->SearchVectors(query, k);
            break;
        }
        // 在此处添加其他索引类型的处理逻辑
        default:
            break;
    }

    // 将结果转换为JSON
    rapidjson::Document json_response;
    json_response.SetObject();
    rapidjson::Document::AllocatorType& allocator = json_response.GetAllocator();

    // 检查是否有有效的搜索结果
    bool valid_results = false;
    rapidjson::Value vectors(rapidjson::kArrayType);
    rapidjson::Value distances(rapidjson::kArrayType);
    for (size_t i = 0; i < results.indices_.size(); ++i) {
        if (results.indices_[i] != -1) {
            valid_results = true;
            vectors.PushBack(results.indices_[i], allocator);
            distances.PushBack(results.distances_[i], allocator);
        }
    }

    if (valid_results) {
        json_response.AddMember(RESPONSE_VECTORS, vectors, allocator);
        json_response.AddMember(RESPONSE_DISTANCES, distances, allocator);
    }

    // 设置响应
    json_response.AddMember(RESPONSE_RETCODE, RESPONSE_RETCODE_SUCCESS, allocator); 
    SetJsonResponse(json_response, res);
}

void HttpServer::InsertHandler(const httplib::Request& req, httplib::Response& res) {
    global_logger->debug("Received insert request");

    // 解析JSON请求
    rapidjson::Document json_request;
    json_request.Parse(req.body.c_str());

    // 打印用户的输入参数
    global_logger->info("Insert request parameters: {}", req.body);

    // 检查JSON文档是否为有效对象
    if (!json_request.IsObject()) {
        global_logger->error("Invalid JSON request");
        res.status = 400;
        SetErrorJsonResponse(res, RESPONSE_RETCODE_ERROR, "Invalid JSON request");
        return;
    }

    // 检查请求的合法性
    if (!IsRequestValid(json_request, CheckType::INSERT)) { // 添加对isRequestValid的调用
        global_logger->error("Missing vectors or id parameter in the request");
        res.status = 400;
        SetErrorJsonResponse(res, RESPONSE_RETCODE_ERROR, "Missing vectors or k parameter in the request");
        return;
    }

    // 获取插入参数
    std::vector<float> data;
    for (const auto& d : json_request[REQUEST_VECTORS].GetArray()) {
        data.push_back(d.GetFloat());
    }
    uint64_t label = json_request[REQUEST_ID].GetUint64(); // 使用宏定义

    global_logger->debug("Insert parameters: label = {}", label);

    // 获取请求参数中的索引类型
    IndexFactory::IndexType index_type = GetIndexTypeFromRequest(json_request);

    // 如果索引类型为UNKNOWN，返回400错误
    if (index_type == IndexFactory::IndexType::UNKNOWN) {
        global_logger->error("Invalid indexType parameter in the request");
        res.status = 400;
        SetErrorJsonResponse(res, RESPONSE_RETCODE_ERROR, "Invalid indexType parameter in the request"); 
        return;
    }

    // 使用全局IndexFactory获取索引对象
    void* index = GetGlobalIndexFactory()->GetIndex(index_type);

    // 根据索引类型初始化索引对象并调用insert_vectors函数
    switch (index_type) {
        case IndexFactory::IndexType::FLAT: {
            FaissIndex* faiss_index = static_cast<FaissIndex*>(index);
            faiss_index->InsertVectors(data, label);
            break;
        }
        // 在此处添加其他索引类型的处理逻辑
        default:
            break;
    }

    // 设置响应
    rapidjson::Document json_response;
    json_response.SetObject();
    rapidjson::Document::AllocatorType& allocator = json_response.GetAllocator();

    // 添加retCode到响应
    json_response.AddMember(RESPONSE_RETCODE, RESPONSE_RETCODE_SUCCESS, allocator);

    SetJsonResponse(json_response, res);
}

void HttpServer::SetJsonResponse(const rapidjson::Document& json_response, httplib::Response& res) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    json_response.Accept(writer);
    res.set_content(buffer.GetString(), RESPONSE_CONTENT_TYPE_JSON); // 使用宏定义
}

void HttpServer::SetErrorJsonResponse(httplib::Response& res, int error_code, const std::string& errorMsg) {
    rapidjson::Document json_response;
    json_response.SetObject();
    rapidjson::Document::AllocatorType& allocator = json_response.GetAllocator();
    json_response.AddMember(RESPONSE_RETCODE, error_code, allocator);
    json_response.AddMember(RESPONSE_ERROR_MSG, rapidjson::StringRef(errorMsg.c_str()), allocator); // 使用宏定义
    SetJsonResponse(json_response, res);
}

} // namespace vdb