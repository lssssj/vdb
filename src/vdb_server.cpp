#include "faiss_index.h"
#include "http_server.h"
#include "index_factory.h"
#include "logger.h"

int main() {
    // 初始化全局日志记录器
    vdb::InitGlobalLogger();
    vdb::SetLogLevel(spdlog::level::debug); // 设置日志级别为debug

    vdb::global_logger->info("Global logger initialized");

    // 初始化全局IndexFactory实例
    int dim = 1; // 向量维度
    vdb::IndexFactory* global_index_factory = vdb::GetGlobalIndexFactory();
    global_index_factory->Init(vdb::IndexFactory::IndexType::FLAT, dim, vdb::IndexFactory::MetricType::L2);
    vdb::global_logger->info("Global IndexFactory initialized");

    // 创建并启动HTTP服务器
    vdb::HttpServer server("localhost", 8080);
    vdb::global_logger->info("HttpServer created");
    server.Start();

    return 0;
}