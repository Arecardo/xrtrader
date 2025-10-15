#include "quote_fetcher.h"
#include "data_struct.h"
#include "log.h"

namespace xrtrader {
namespace data {

LongportQuoteFetcher::LongportQuoteFetcher() {}
LongportQuoteFetcher::~LongportQuoteFetcher() {}

bool LongportQuoteFetcher::Init(const longport::Config& config, std::shared_ptr<QuoteDataStore> data_store) {
    log_info("LongportQuoteFetcher Init");
    if (_quote_ctx.ref_count() > 0)
    {
        log_warn("QuoteContext already initialized");
        return true;
    }

    if (data_store == nullptr) {
        log_error("Data store is null");
        return false;
    }
    else {
        _data_store = data_store;
    }

    longport::quote::QuoteContext::create(config, [this](auto res) {
        if (!res) {
            log_error("failed to create quote context. error_code:{} error_info:{}", res.status().code(), res.status().message());
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
    
    if (symbols.empty()) {
        log_warn("No symbols provided for subscription");
        return false;
    }

    if (_quote_ctx.ref_count() == 0) {
        log_error("QuoteContext is not initialized");
        return false;
    }

    _quote_ctx.set_on_quote([this](auto event) {
        log_debug("Received quote for symbol: {}, timestamp: {}, last_done: {}, open: {}, high: {}, low: {}, volume: {}, turnover: {}",
                 event->symbol, event->timestamp, (double)event->last_done, (double)event->open,
                 (double)event->high, (double)event->low, event->volume, (double)event->turnover);
        
        // Store the quote data
        auto quote_data = std::make_shared<SecurityQuoteData>();
        quote_data->symbol = event->symbol;
        quote_data->last_price = (double)event->last_done;
        quote_data->open = (double)event->open;
        quote_data->high = (double)event->high;
        quote_data->low = (double)event->low;
        quote_data->volume = event->volume;
        quote_data->timestamp = std::chrono::system_clock::from_time_t(event->timestamp);

        _data_store->update_quote(quote_data);
    });

    _quote_ctx.subscribe(symbols, longport::quote::SubFlags::QUOTE(), true, [](auto res) {
      if (!res) {
        log_error("failed to subscribe quote: {}", res.status().message());
        return;
      }
    });
    
    return true;
}

bool LongportQuoteFetcher::SubscribeCandlestick(const std::string& symbol, longport::quote::Period period) {
    log_info("SubscribeCandlestick called, symbol: {}", symbol);
    if (symbol.empty()) {
        log_warn("No symbol provided for SubscribeCandlestick");
        return false;
    }

    if (period == longport::quote::Period::Unknown) {
        log_warn("Invalid period provided for SubscribeCandlestick");
        return false;
    }

    if (_quote_ctx.ref_count() == 0) {
        log_error("QuoteContext is not initialized");
        return false;
    }
    _quote_ctx.set_on_candlestick([this](auto event) {

    });

    return true;
}

bool LongportQuoteFetcher::GetQuote(const std::vector<std::string>& symbols) {
    log_info("GetQuote called, symbols size: {}", symbols.size());
    if (_quote_ctx.ref_count() == 0) {
        log_error("QuoteContext is not initialized");
        return false;
    }

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

bool LongportQuoteFetcher::GetCandlestick(const std::string& symbol, longport::quote::Period period) {
    log_info("GetCandlestick called, symbol: {}", symbol);
    if (_quote_ctx.ref_count() == 0) {
        log_error("QuoteContext is not initialized");
        return false;
    }

    return true;
}

} // namespace data
} // namespace xrtrader
