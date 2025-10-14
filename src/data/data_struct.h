#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <deque>
#include <mutex>
#include <shared_mutex>
#include "longport.hpp"

namespace xrtrader {
namespace data {

// 实时行情数据结构
struct SecurityQuoteData {
    std::string symbol;
    double last_price = 0.0;
    uint64_t volume = 0;
    double high = 0.0;
    double low = 0.0;
    double open = 0.0;
    double bid_price = 0.0;
    double ask_price = 0.0;
    uint64_t bid_volume = 0;
    uint64_t ask_volume = 0;
    std::chrono::system_clock::time_point timestamp;
    
    // 计算相关指标
    double price_change() const { 
        return open != 0.0 ? (last_price - open) / open : 0.0; 
    }
};

// 历史K线数据结构
struct CandlestickData {
    std::string symbol;
    std::chrono::system_clock::time_point timestamp;
    double open = 0.0;
    double high = 0.0;
    double low = 0.0;
    double close = 0.0;
    uint64_t volume = 0;
    double turnover = 0.0;
};

// 时间序列数据容器
template<typename T>
class TimeSeriesContainer {
private:
    std::deque<T> data_;
    size_t max_size_;
    mutable std::shared_mutex mutex_;

public:
    explicit TimeSeriesContainer(size_t max_size = 1000) : max_size_(max_size) {}
    
    void add(const T& item) {
        std::unique_lock lock(mutex_);
        data_.push_back(item);
        if (data_.size() > max_size_) {
            data_.pop_front();
        }
    }
    
    std::vector<T> get_latest(size_t count) const {
        std::shared_lock lock(mutex_);
        std::vector<T> result;
        size_t start = data_.size() > count ? data_.size() - count : 0;
        for (size_t i = start; i < data_.size(); ++i) {
            result.push_back(data_[i]);
        }
        return result;
    }
    
    std::vector<T> get_all() const {
        std::shared_lock lock(mutex_);
        return std::vector<T>(data_.begin(), data_.end());
    }
    
    size_t size() const {
        std::shared_lock lock(mutex_);
        return data_.size();
    }
};

// 账户余额数据结构
struct AccountBalance {
    std::string currency;
    double cash_balance = 0.0;
    double available_cash = 0.0;
    double market_value = 0.0;
    double total_assets = 0.0;
    std::chrono::system_clock::time_point timestamp;
};

// 持仓数据结构
struct Position {
    std::string symbol;
    std::string symbol_name;
    double quantity = 0.0;
    double available_quantity = 0.0;
    double cost_price = 0.0;
    double market_price = 0.0;
    double market_value = 0.0;
    double profit_loss = 0.0;
    double profit_loss_ratio = 0.0;
    longport::Market market;
    std::chrono::system_clock::time_point timestamp;
    
    // 计算盈亏
    void calculate_profit_loss() {
        if (cost_price > 0) {
            profit_loss = (market_price - cost_price) * quantity;
            profit_loss_ratio = (market_price - cost_price) / cost_price;
        }
    }
};

// 账户持仓频道
struct PositionChannel {
    std::string account_channel;
    std::vector<Position> positions;
};

} // namespace data
} // namespace xrtrader