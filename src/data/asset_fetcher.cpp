#include "asset_fetcher.h"
#include <spdlog/spdlog.h>

LongportAssetFetcher::LongportAssetFetcher() {}
LongportAssetFetcher::~LongportAssetFetcher() {}

bool LongportAssetFetcher::Init() {
    log_info("LongportAssetFetcher Init called");
    // TODO: 初始化longport sdk上下文
    return true;
}

bool LongportAssetFetcher::GetAsset() {
    log_info("GetAsset called");
    // TODO: 调用longport sdk获取资产
    return true;
}

bool LongportAssetFetcher::GetPosition() {
    log_info("GetPosition called");
    // TODO: 调用longport sdk获取持仓
    return true;
}
