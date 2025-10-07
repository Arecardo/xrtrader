#pragma once
#include <spdlog/spdlog.h>

#define log_debug(...) SPDLOG_DEBUG(__VA_ARGS__)
#define log_info(...) SPDLOG_INFO(__VA_ARGS__)
#define log_warn(...) SPDLOG_WARN(__VA_ARGS__)
#define log_error(...) SPDLOG_ERROR(__VA_ARGS__)
#define log_critical(...) SPDLOG_CRITICAL(__VA_ARGS__)
