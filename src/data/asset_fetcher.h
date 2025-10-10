#pragma once

#include "longport.hpp"
#include <future>

namespace xrtrader {
namespace data {

class IAssetFetcher {
public:
    virtual ~IAssetFetcher() = default;
    virtual bool Init(const longport::Config& config) = 0;
    virtual bool GetAsset() = 0;
    virtual bool GetPosition() = 0;
};

class LongportAssetFetcher : public IAssetFetcher {
public:
    LongportAssetFetcher();
    ~LongportAssetFetcher();
    bool Init(const longport::Config& config) override;
    bool GetAsset() override;
    bool GetPosition() override;

private:
    // longport sdk context members
    longport::trade::TradeContext _trade_ctx;
};

} // namespace data
} // namespace xrtrader