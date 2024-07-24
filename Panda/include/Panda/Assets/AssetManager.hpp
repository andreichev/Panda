#pragma once

namespace Panda {

class AssetManager {
public:
private:
    std::unordered_map<AssetHandle, Ref<Asset>> m_LoadedAssets;
};

} // namespace Panda