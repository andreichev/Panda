#pragma once

#include <Panda/Assets/TextureAsset.hpp>

namespace Panda {

class ThumbnailProvider final {
public:
    Foundation::Shared<TextureAsset> getThumbnailOrNull(AssetId assetId, Size size);
    void resetCache();

private:
    std::unordered_map<AssetId, Foundation::Shared<TextureAsset>> m_cache;
};

} // namespace Panda