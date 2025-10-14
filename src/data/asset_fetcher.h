#pragma once

#include "longport.hpp"
#include "data_store.h"
#include <future>

namespace xrtrader {
namespace data {

class IAssetFetcher {
public:
    virtual ~IAssetFetcher() = default;
};

class LongportAssetFetcher : public IAssetFetcher {
public:
    LongportAssetFetcher();
    ~LongportAssetFetcher();
    bool Init(const longport::Config& config, std::shared_ptr<AssetDataStore> data_store);
    bool GetAsset();
    bool GetPosition();

private:
    // longport sdk context members
    longport::trade::TradeContext _trade_ctx;

    std::shared_ptr<AssetDataStore> _data_store;
};

} // namespace data
} // namespace xrtrader