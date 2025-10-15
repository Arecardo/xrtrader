#include "asset_fetcher.h"
#include "data_struct.h"
#include "log.h"
#include "longport.hpp"

namespace xrtrader {
namespace data {

LongportAssetFetcher::LongportAssetFetcher() {}
LongportAssetFetcher::~LongportAssetFetcher() {}

bool LongportAssetFetcher::Init(const longport::Config& config, std::shared_ptr<AssetDataStore> data_store) {
    log_info("LongportAssetFetcher Init called");
    if (_trade_ctx.ref_count() > 0)
    {
        log_warn("TradeContext already initialized");
        return true;
    }

    if (!data_store) {
        log_error("Data store is null");
        return false;
    }
    else
    {
        _data_store = data_store;
    }
    
    // 使用信号量等待初始化完成
    std::mutex mtx;
    std::condition_variable cv;
    bool initialized = false;
    uint64_t error_code = 0;
    std::string error_message;
    
    // 初始化longport sdk交易上下文
    longport::trade::TradeContext::create(config, [this, &mtx, &cv, &initialized, &error_code, &error_message](auto res) {
        std::lock_guard<std::mutex> lock(mtx);
        if (!res) {
            error_code = res.status().code();
            error_message = res.status().message();
            initialized = true;
            cv.notify_one();
            return;
        }
        _trade_ctx = res.context();
        initialized = true;
        cv.notify_one();
    });

    // 等待初始化完成
    std::unique_lock<std::mutex> lock(mtx);
    if (!cv.wait_for(lock, std::chrono::seconds(10), [&] { return initialized; })) {
        log_error("TradeContext initialization timeout");
        return false;
    }
    
    if (!error_message.empty()) {
        log_error("Failed to create trade context. error_code:{}, error_info:{}", error_code, error_message);
        return false;
    }
    
    log_info("TradeContext created successfully");
    return true;
}

bool LongportAssetFetcher::GetAsset() {
    log_info("GetAsset called");
    
    // 调用longport sdk获取账户余额
    const std::string currency = "USD";
    _trade_ctx.account_balance(currency, [](auto res) {
        if (!res) {
            log_error("Failed to get account balance: {}", res.status().message());
            // 在出现错误的情况下，设置一个空的 vector
            return;
        }
        
        log_info("Account Balance size:{}", res->size());

        std::vector<AccountBalance> balances;
        for (auto it = res->begin(); it != res->end(); ++it) {
            AccountBalance balance;
            balance.currency = it->currency;
            balance.cash_balance = (double)it->total_cash;
            balance.timestamp = std::chrono::system_clock::now();
            
            log_info("Total cash: {}, currency: {}", (double)it->total_cash, it->currency);
        }
    });
    
    return true;
}

bool LongportAssetFetcher::GetPosition() {
    log_info("GetPosition called");
    const std::optional<longport::trade::GetStockPositionsOptions> opts = std::nullopt;
    _trade_ctx.stock_positions(opts, [](auto res) { 
        if (!res) {
            log_error("Failed to get stock positions: {}", res.status().message());
            return;
        }

        std::vector<longport::trade::StockPositionChannel> channels = res->channels;
        log_info("Stock Position Channels size:{}", channels.size());
        for (auto it = channels.begin(); it != channels.end(); ++it) {
            log_info("account channle type: {}, positions size: {}", it->account_channel, it->positions.size());
            for (auto pos_it = it->positions.begin(); pos_it != it->positions.end(); ++pos_it) {
                log_info("symbol: {}, symbol_name: {}, quantity: {}, available_quantity: {}, cost_price: {}, market: {}",
                         pos_it->symbol, pos_it->symbol_name, (double)pos_it->quantity, (double)pos_it->available_quantity,
                         (double)pos_it->cost_price, static_cast<int>(pos_it->market));
            }
        }
    });
    
    // 调用longport sdk获取股票持仓
    return true;
}

} // namespace data
} // namespace xrtrader