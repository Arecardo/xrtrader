#pragma once
#include <string>
#include <vector>
#include <memory>
#include "longport.hpp"
#include "log.h"

using namespace longport;

class IAssetFetcher {
public:
    virtual ~IAssetFetcher() = default;
};

class LongportAssetFetcher : public IAssetFetcher {
public:
    LongportAssetFetcher();
    ~LongportAssetFetcher();
    bool Init(const longport::Config& config);
    bool GetAsset();
    bool GetPosition();
private:
    // longport sdk context members
    void* quote_ctx_ = nullptr;
    void* trade_ctx_ = nullptr;
};
