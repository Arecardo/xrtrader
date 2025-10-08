#include "quote_fetcher.h"
#include <spdlog/spdlog.h>

namespace xrtrader {
namespace data {

LongportQuoteFetcher::LongportQuoteFetcher() {}
LongportQuoteFetcher::~LongportQuoteFetcher() {}

bool LongportQuoteFetcher::Init(const longport::Config& config) {
    log_info("LongportQuoteFetcher Init");
    if (_quote_ctx.ref_count() > 0)
    {
        log_warn("QuoteContext already initialized");
        return true;
    }

    longport::quote::QuoteContext::create(config, [this](auto res) {
        if (!res) {
            log_error("failed to create quote context: {}", res.status().message());
            return;
        }
        _quote_ctx = res.context();
        log_info("QuoteContext created successfully");
    });

    while (_quote_ctx.ref_count() == 0) {
        // wait for initializationll
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return true;
}

bool LongportQuoteFetcher::Subscribe(const std::vector<std::string>& symbols) {
    log_info("Subscribe called, symbols size: {}", symbols.size());
    // TODO: 调用longport sdk订阅行情
    return true;
}

bool LongportQuoteFetcher::GetQuote(const std::vector<std::string>& symbols) {
    log_info("GetQuote called, symbols size: {}", symbols.size());
    if (!symbols.empty()) {
        _quote_ctx.quote(symbols, [](auto res) {
            if (!res) {
                log_error("failed to get quotes: {}", res.status().message());
                return;
            }

            for (auto it = res->cbegin(); it != res->cend(); ++it) {
                log_info("Symbol: {}, Open: {}, High: {}, Low: {}, Volume: {}",
                        it->symbol, (double)it->open, (double)it->high, (double)it->low, it->volume);
            }
        });
    } else {
        log_warn("No symbols provided for GetQuote");
    }
    return true;
}

} // namespace data
} // namespace xrtrader
