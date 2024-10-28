//
// Created by Michael Andreichev on 14.10.2024.
//

#include "Panda/Assets/AssetManager.hpp"

#include <unordered_map>

namespace Panda {

class EditorAssetManager : public AssetManager {
public:
private:
    std::unordered_map<AssetHandle, Foundation::Shared<Asset>> m_loadedAssets;
};

} // namespace Panda
