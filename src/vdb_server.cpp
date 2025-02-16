#include "faiss_index.h"
#include "http_server.h"
#include "index_factory.h"
#include "logger.h"

int main() {
    // 初始化全局日志记录器
    vdb::init_global_logger();
    vdb::set_log_level(spdlog::level::debug); // 设置日志级别为debug

    vdb::GlobalLogger->info("Global logger initialized");

    // 初始化全局IndexFactory实例
    int dim = 1; // 向量维度
    vdb::IndexFactory* globalIndexFactory = vdb::getGlobalIndexFactory();
    globalIndexFactory->init(vdb::IndexFactory::IndexType::FLAT, dim, vdb::IndexFactory::MetricType::L2);
    vdb::GlobalLogger->info("Global IndexFactory initialized");

    // 创建并启动HTTP服务器
    vdb::HttpServer server("localhost", 8080);
    vdb::GlobalLogger->info("HttpServer created");
    server.start();

    return 0;
}