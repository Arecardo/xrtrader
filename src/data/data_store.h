#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <shared_mutex>
#include "data_struct.h"

namespace xrtrader {
namespace data {

class QuoteDataStore {
public:
    QuoteDataStore() = default;
    ~QuoteDataStore() = default;
    
    // 存储实时行情数据
    void update_quote(const std::shared_ptr<SecurityQuoteData>& quote);
    
    // 获取最新行情数据
    std::shared_ptr<SecurityQuoteData> get_latest_quote(const std::string& symbol) const;
    
    // 存储K线数据
    void add_candlestick(const std::shared_ptr<CandlestickData>& candlestick);
    
    // 获取K线数据
    std::shared_ptr<TimeSeriesContainer<CandlestickData>> get_candlestick_data(const std::string& symbol) const;
    
    // 计算简单移动平均线
    double calculate_sma(const std::string& symbol, size_t period) const;

private:
    // 实时行情数据存储
    mutable std::shared_mutex quote_mutex_;
    std::unordered_map<std::string, std::shared_ptr<SecurityQuoteData>> quote_data_;
    
    // 历史K线数据存储
    mutable std::shared_mutex candlestick_mutex_;
    std::unordered_map<std::string, std::shared_ptr<TimeSeriesContainer<CandlestickData>>> candlestick_data_;
};

class AssetDataStore {
public:
    AssetDataStore() = default;
    ~AssetDataStore() = default;
    
    // 更新账户余额
    void update_account_balances(const std::vector<AccountBalance>& balances);
    
    // 更新持仓信息
    void update_position_channels(const std::vector<PositionChannel>& channels);
    
    // 获取账户余额
    std::vector<AccountBalance> get_account_balances() const;
    
    // 获取持仓信息
    std::vector<PositionChannel> get_position_channels() const;
    
    // 根据股票代码获取持仓
    std::shared_ptr<Position> get_position_by_symbol(const std::string& symbol) const;

private:
    // 账户数据存储
    mutable std::shared_mutex account_mutex_;
    std::vector<AccountBalance> account_balances_;
    
    // 持仓数据存储
    mutable std::shared_mutex position_mutex_;
    std::vector<PositionChannel> position_channels_;
};

} // namespace data
} // namespace xrtrader