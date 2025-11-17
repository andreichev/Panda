#pragma once

#include "Panda/Assets/MaterialAsset.hpp"

namespace Panda {

class FallbackTextureAsset final : public TextureAsset {
public:
    FallbackTextureAsset();
};

class FallbackMaterialAsset final : public MaterialAsset {
public:
    FallbackMaterialAsset();
};

} // namespace Panda