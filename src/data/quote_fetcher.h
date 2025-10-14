#pragma once
#include <string>
#include <vector>
#include <memory>
#include "data_store.h"
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
    bool Init(const longport::Config& config, std::shared_ptr<QuoteDataStore> quote_store);
    bool Subscribe(const std::vector<std::string>& symbols);
    bool GetQuote(const std::vector<std::string>& symbols);
private:
    // longport sdk context members
    longport::quote::QuoteContext _quote_ctx;

    // Data store for storing fetched data
    std::shared_ptr<data::QuoteDataStore> _data_store;
};
} // namespace data
} // namespace xrtrader
