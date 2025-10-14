
#include <iostream>
#include "data/asset_fetcher.h"
#include "data/quote_fetcher.h"
#include "data/data_store.h"
#include "data/data_struct.h"
#include "longport.hpp"
#include "log.h"
#include <vector>
#include <string>

using namespace longport;
using namespace xrtrader::data;

int main() {
    log_info("Welcome to XRTrader!");

    Config config;
    Status status = Config::from_env(config);
    if (!status) {
        std::cout << "failed to load configuration from environment: "
                << status.message() << std::endl;
        return -1;
    }

    auto asset_data_store = std::make_shared<AssetDataStore>();
    auto quote_data_store = std::make_shared<QuoteDataStore>();

    LongportAssetFetcher asset_fetcher;
    if (!asset_fetcher.Init(config, asset_data_store)) {
        log_error("资产获取器初始化失败");
        return 1;
    }
    asset_fetcher.GetAsset();
    asset_fetcher.GetPosition();

    LongportQuoteFetcher quote_fetcher;
    if (!quote_fetcher.Init(config, quote_data_store)) {
        log_error("行情获取器初始化失败");
        return 1;
    }
    std::vector<std::string> symbols = {"TSLA.US"};
    quote_fetcher.Subscribe(symbols);
    quote_fetcher.GetQuote(symbols);

    std::cin.get();
    log_info("测试完成");
    return 0;
}