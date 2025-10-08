#pragma once
#include <string>
#include <vector>
#include <memory>
#include "longport.hpp"
#include "log.h"

namespace xrtrader {
namespace data {

class IQuoteFetcher {
public:
    virtual ~IQuoteFetcher() = default;
};

class LongportQuoteFetcher : public IQuoteFetcher {
public:
    LongportQuoteFetcher();
    ~LongportQuoteFetcher();
    bool Init(const longport::Config& config);
    bool Subscribe(const std::vector<std::string>& symbols);
    bool GetQuote(const std::vector<std::string>& symbols);
private:
    // longport sdk context members
    longport::quote::QuoteContext _quote_ctx;
};
} // namespace data
} // namespace xrtrader
