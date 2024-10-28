#pragma once

#include "Asset.hpp"

#include <unordered_set>

namespace Panda {

class AssetManager {
public:
    virtual ~AssetManager() = default;

    virtual Asset getAsset(AssetHandle assetHandle) = 0;
    virtual bool reloadData(AssetHandle assetHandle) = 0;
    virtual bool isValid(AssetHandle assetHandle) = 0;
    virtual bool isAssetLoaded(AssetHandle handle) = 0;
};

} // namespace Panda