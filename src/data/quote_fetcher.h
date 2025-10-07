#pragma once
#include <string>
#include <vector>
#include <memory>
#include "log.h"

class IQuoteFetcher {
public:
    virtual ~IQuoteFetcher() = default;
    virtual bool Init() = 0;
};

class LongportQuoteFetcher : public IQuoteFetcher {
public:
    LongportQuoteFetcher();
    ~LongportQuoteFetcher();
    bool Init() override;
    bool Subscribe(const std::vector<std::string>& symbols);
    bool GetQuote(const std::vector<std::string>& symbols);
private:
    // longport sdk context members
    void* quote_ctx_ = nullptr;
    void* trade_ctx_ = nullptr;
};
