#include "data_store.h"
#include <numeric>
#include <mutex>

namespace xrtrader {
namespace data {

void QuoteDataStore::update_quote(const std::shared_ptr<SecurityQuoteData>& quote) {
    std::unique_lock lock(quote_mutex_);
    quote_data_[quote->symbol] = quote;
}

std::shared_ptr<SecurityQuoteData> QuoteDataStore::get_latest_quote(const std::string& symbol) const {
    std::shared_lock lock(quote_mutex_);
    auto it = quote_data_.find(symbol);
    if (it != quote_data_.end()) {
        return it->second;
    }
    return nullptr;
}

void QuoteDataStore::add_candlestick(const std::shared_ptr<CandlestickData>& candlestick) {
    std::unique_lock lock(candlestick_mutex_);
    auto& data = candlestick_data_[candlestick->symbol];
    if (!data) {
        data = std::make_shared<TimeSeriesContainer<CandlestickData>>(1000);
    }
    data->add(*candlestick);
}

std::shared_ptr<TimeSeriesContainer<CandlestickData>> QuoteDataStore::get_candlestick_data(const std::string& symbol) const {
    std::shared_lock lock(candlestick_mutex_);
    auto it = candlestick_data_.find(symbol);
    if (it != candlestick_data_.end()) {
        return it->second;
    }
    return nullptr;
}

double QuoteDataStore::calculate_sma(const std::string& symbol, size_t period) const {
    auto candlestick_data = get_candlestick_data(symbol);
    if (!candlestick_data || candlestick_data->size() < period) {
        return 0.0;
    }
    
    auto latest_candlesticks = candlestick_data->get_latest(period);
    if (latest_candlesticks.empty()) {
        return 0.0;
    }
    
    double sum = std::accumulate(latest_candlesticks.begin(), latest_candlesticks.end(), 0.0,
                                [](double acc, const CandlestickData& candle) {
                                    return acc + candle.close;
                                });
    
    return sum / latest_candlesticks.size();
}

void AssetDataStore::update_account_balances(const std::vector<AccountBalance>& balances) {
    std::unique_lock lock(account_mutex_);
    account_balances_ = balances;
}

void AssetDataStore::update_position_channels(const std::vector<PositionChannel>& channels) {
    std::unique_lock lock(position_mutex_);
    position_channels_ = channels;
}

std::vector<AccountBalance> AssetDataStore::get_account_balances() const {
    std::shared_lock lock(account_mutex_);
    return account_balances_;
}

std::vector<PositionChannel> AssetDataStore::get_position_channels() const {
    std::shared_lock lock(position_mutex_);
    return position_channels_;
}

std::shared_ptr<Position> AssetDataStore::get_position_by_symbol(const std::string& symbol) const {
    std::shared_lock lock(position_mutex_);
    for (const auto& channel : position_channels_) {
        for (const auto& position : channel.positions) {
            if (position.symbol == symbol) {
                auto pos_copy = std::make_shared<Position>(position);
                return pos_copy;
            }
        }
    }
    return nullptr;
}

} // namespace data
} // namespace xrtrader