#include "Panda/Assets/StaticResources.hpp"
#include "Panda/GameLogic/GameContext.hpp"

namespace Panda {

namespace StaticResources {

    void initStaticResources() {
        AssetHandler *handler = GameContext::getAssetHandler();
        MaterialData materialData;
        /// WHITE TEXTURE
        Foundation::Memory whiteTextureData = Foundation::Memory::alloc(sizeof(uint32_t));
        *(uint32_t *)whiteTextureData.data = 0xffffffff;
        Miren::TextureCreate whiteTextureCreate;
        whiteTextureCreate.m_data = whiteTextureData;
        whiteTexture = handler->createStaticAsset<TextureAsset>(1, whiteTextureCreate);
        materialData.inputs = {
            MaterialField("albedo", MaterialFieldType::TEXTURE, whiteTexture.asBaseAsset())
        };
        /// DEFAULT SHADER
        defaultShader = handler->createStaticAsset<ShaderAsset>(2, "default-shaders/default.frag");
        /// DEFAULT MATERIAL
        defaultMaterial = handler->createStaticAsset<MaterialAsset>(3, materialData, defaultShader);
        /// SELECTED GEOMETRY SHADER
        selectedGeometryShader =
            handler->createStaticAsset<ShaderAsset>(4, "default-shaders/selection_map.frag");
        /// SELECTED GEOMETRY MATERIAL
        selectedGeometryMaterial =
            handler->createStaticAsset<MaterialAsset>(5, materialData, selectedGeometryShader);
    }

    void deinitStaticResources() {
        whiteTexture.reset();
        defaultShader.reset();
        selectedGeometryShader.reset();
        defaultMaterial.reset();
        selectedGeometryMaterial.reset();
    }

    AssetRef<TextureAsset> whiteTexture;
    AssetRef<ShaderAsset> defaultShader;
    AssetRef<ShaderAsset> selectedGeometryShader;
    AssetRef<MaterialAsset> defaultMaterial;
    AssetRef<MaterialAsset> selectedGeometryMaterial;

} // namespace StaticResources

} // namespace Panda