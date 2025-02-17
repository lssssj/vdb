#pragma once

#include "spdlog/spdlog.h"

namespace vdb {

extern std::shared_ptr<spdlog::logger> global_logger;

void InitGlobalLogger();
void SetLogLevel(spdlog::level::level_enum log_level);

} // namespace vdb