#pragma once

#include <string>

#include "faiss_index.h"
#include "httplib.h"
#include "index_factory.h"
#include <rapidjson/document.h>

namespace vdb {
class HttpServer {
public:
    enum class CheckType {
        SEARCH,
        INSERT 
    };

    HttpServer(const std::string& host, int port);
    void Start();

private:
    void SearchHandler(const httplib::Request& req, httplib::Response& res);
    void InsertHandler(const httplib::Request& req, httplib::Response& res);
    void SetJsonResponse(const rapidjson::Document& json_response, httplib::Response& res);
    void SetErrorJsonResponse(httplib::Response& res, int error_code, const std::string& errorMsg); 
    bool IsRequestValid(const rapidjson::Document& json_request, CheckType check_type);
    IndexFactory::IndexType GetIndexTypeFromRequest(const rapidjson::Document& json_request); 

    httplib::Server server_;
    std::string host_;
    int port_;
};

} // namespace vdb