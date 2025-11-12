#include "ThumbnailProvider.hpp"
#include "ProjectLoader/AssetHandlerEditor.hpp"

#include <Panda/GameLogic/GameContext.hpp>

namespace Panda {

Foundation::Shared<Thumbnail> ThumbnailProvider::getThumbnailOrNull(AssetId assetId, Size size) {
    if (m_cache.find(assetId) != m_cache.end()) { return m_cache.at(assetId); }
    if (!GameContext::getAssetHandler()) { return nullptr; }
    AssetHandlerEditor *assetHandler =
        static_cast<AssetHandlerEditor *>(GameContext::getAssetHandler());
    AssetInfo info = assetHandler->getInfo(assetId);
    Foundation::Shared<Thumbnail> thumbnail = nullptr;
    switch (info.type) {
        case AssetType::TEXTURE: {
            TextureAssetMeta meta = std::get<TextureAssetMeta>(info.meta);
            path_t path = assetHandler->getProjectPath() / meta.path;
            if (std::filesystem::exists(path)) {
                Miren::TextureCreate create = AssetImporterBase::load2DTexture(path);
                thumbnail = Foundation::makeShared<Thumbnail>(create);
            }
            break;
        }
        case AssetType::CUBE_MAP:
        case AssetType::SHADER:
        case AssetType::MATERIAL:
        case AssetType::MESH:
        case AssetType::NONE:
            break;
    }
    m_cache[assetId] = thumbnail;
    return thumbnail;
}

void ThumbnailProvider::resetCache() {
    m_cache.clear();
}

} // namespace Panda