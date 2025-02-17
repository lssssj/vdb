#include "logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace vdb {

std::shared_ptr<spdlog::logger> global_logger;

void InitGlobalLogger() {
    global_logger = spdlog::stdout_color_mt("GlobalLogger");
}

void SetLogLevel(spdlog::level::level_enum log_level) {
    global_logger->set_level(log_level);
}

} // namespace vdb