#include "quote_fetcher.h"
#include <spdlog/spdlog.h>

LongportQuoteFetcher::LongportQuoteFetcher() {}
LongportQuoteFetcher::~LongportQuoteFetcher() {}

bool LongportQuoteFetcher::Init() {
    log_info("LongportQuoteFetcher Init called");
    // TODO: 初始化longport sdk上下文
    return true;
}

bool LongportQuoteFetcher::Subscribe(const std::vector<std::string>& symbols) {
    log_info("Subscribe called, symbols size: {}", symbols.size());
    // TODO: 调用longport sdk订阅行情
    return true;
}

bool LongportQuoteFetcher::GetQuote(const std::vector<std::string>& symbols) {
    log_info("GetQuote called, symbols size: {}", symbols.size());
    // TODO: 调用longport sdk获取行情
    return true;
}
