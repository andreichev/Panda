#include "ThumbnailProvider.hpp"

#include <Panda/GameLogic/GameContext.hpp>

namespace Panda {

Foundation::Shared<TextureAsset> ThumbnailProvider::getThumbnailOrNull(AssetId assetId, Size size) {
    if (m_cache.find(assetId) != m_cache.end()) { return m_cache.at(assetId); }
    Foundation::Shared<Asset> asset = GameContext::s_assetHandler->load(assetId);
    Foundation::Shared<TextureAsset> thumbnail = nullptr;
    switch (asset->getType()) {
        case AssetType::TEXTURE: {
            thumbnail = Foundation::SharedCast<TextureAsset>(asset);
            break;
        }
        case AssetType::NONE:
        case AssetType::CUBE_MAP:
        case AssetType::SHADER:
        case AssetType::MATERIAL:
            break;
    }
    if (thumbnail) { m_cache[assetId] = thumbnail; }
    return thumbnail;
}

void ThumbnailProvider::resetCache() {
    m_cache.clear();
}

} // namespace Panda