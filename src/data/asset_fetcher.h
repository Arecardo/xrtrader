#pragma once
#include <string>
#include <vector>
#include <memory>
#include "log.h"

class IAssetFetcher {
public:
    virtual ~IAssetFetcher() = default;
    virtual bool Init() = 0;
};

class LongportAssetFetcher : public IAssetFetcher {
public:
    LongportAssetFetcher();
    ~LongportAssetFetcher();
    bool Init() override;
    bool GetAsset();
    bool GetPosition();
private:
    // longport sdk context members
    void* quote_ctx_ = nullptr;
    void* trade_ctx_ = nullptr;
};
