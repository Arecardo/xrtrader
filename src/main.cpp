
#include <iostream>
#include "data/asset_fetcher.h"
#include "data/quote_fetcher.h"
#include "log.h"
#include <vector>
#include <string>

int main() {
    log_info("程序启动");

    LongportAssetFetcher asset_fetcher;
    if (!asset_fetcher.Init()) {
        log_error("资产获取器初始化失败");
        return 1;
    }
    asset_fetcher.GetAsset();
    asset_fetcher.GetPosition();

    LongportQuoteFetcher quote_fetcher;
    if (!quote_fetcher.Init()) {
        log_error("行情获取器初始化失败");
        return 1;
    }
    std::vector<std::string> symbols = {"AAPL.US", "TSLA.US"};
    quote_fetcher.Subscribe(symbols);
    quote_fetcher.GetQuote(symbols);

    log_info("测试完成");
    return 0;
}