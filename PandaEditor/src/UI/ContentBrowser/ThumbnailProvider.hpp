#pragma once

#include <Panda/Assets/TextureAsset.hpp>

namespace Panda {

class ThumbnailProvider final {
public:
    Foundation::Shared<TextureAsset> getThumbnailOrNull(AssetId assetId);
};

} // namespace Panda