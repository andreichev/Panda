#include "ThumbnailProvider.hpp"

#include <Panda/GameLogic/GameContext.hpp>

namespace Panda {

Foundation::Shared<TextureAsset> ThumbnailProvider::getThumbnailOrNull(AssetId assetId) {
    auto asset = GameContext::s_assetHandler->load(assetId);
    switch (asset->getType()) {
        case AssetType::TEXTURE: {
            return Foundation::SharedCast<TextureAsset>(asset);
        }
        case AssetType::NONE:
        case AssetType::CUBE_MAP:
        case AssetType::PROGRAM:
            break;
    }
    return nullptr;
}

} // namespace Panda