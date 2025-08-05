#include "ThumbnailProvider.hpp"
#include "ProjectLoader/AssetHandlerEditor.hpp"

#include <Panda/GameLogic/GameContext.hpp>

namespace Panda {

Foundation::Shared<TextureAsset> ThumbnailProvider::getThumbnailOrNull(AssetId assetId, Size size) {
    if (m_cache.find(assetId) != m_cache.end()) { return m_cache.at(assetId); }
    if (!GameContext::getAssetHandler()) { return nullptr; }
    AssetHandlerEditor *assetHandler =
        static_cast<AssetHandlerEditor *>(GameContext::getAssetHandler());
    AssetInfo info = assetHandler->getInfo(assetId);
    Foundation::Shared<TextureAsset> thumbnail = nullptr;
    switch (info.type) {
        case AssetType::TEXTURE: {
            auto asset = assetHandler->load(assetId);
            thumbnail = Foundation::SharedCast<TextureAsset>(asset);
            break;
        }
        case AssetType::NONE:
        case AssetType::CUBE_MAP:
        case AssetType::SHADER:
        case AssetType::MATERIAL:
            break;
    }
    m_cache[assetId] = thumbnail;
    return thumbnail;
}

void ThumbnailProvider::resetCache() {
    m_cache.clear();
}

} // namespace Panda