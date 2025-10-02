#pragma once

#include "Panda/Assets/Base/AssetRef.hpp"
#include "Panda/Assets/MaterialAsset.hpp"
#include "Panda/Assets/MeshAsset.hpp"
#include "Panda/Assets/TextureAsset.hpp"
#include "Panda/Assets/ShaderAsset.hpp"

namespace Panda {

namespace StaticResources {

    void initStaticResources();
    void deinitStaticResources();

    extern AssetRef<TextureAsset> whiteTexture;
    extern AssetRef<ShaderAsset> defaultShader;
    extern AssetRef<ShaderAsset> selectedGeometryShader;
    extern AssetRef<MaterialAsset> defaultMaterial;
    extern AssetRef<MaterialAsset> selectedGeometryMaterial;

} // namespace StaticResources

} // namespace Panda