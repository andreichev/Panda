#include "Panda/Assets/StaticResources.hpp"
#include "Panda/GameLogic/GameContext.hpp"

namespace Panda {

namespace StaticResources {

    void initStaticResources() {
        AssetHandler *handler = GameContext::getAssetHandler();
        MaterialData materialData;
        defaultShader =
            handler->createStaticAsset<ShaderAsset>(1, "default-shaders/default_fragment.glsl");
        defaultMaterial = handler->createStaticAsset<MaterialAsset>(2, materialData, defaultShader);
        selectedGeometryShader = handler->createStaticAsset<ShaderAsset>(
            3, "default-shaders/selection_map_fragment.glsl"
        );
        selectedGeometryMaterial =
            handler->createStaticAsset<MaterialAsset>(4, materialData, selectedGeometryShader);
    }

    AssetRef<ShaderAsset> defaultShader;
    AssetRef<ShaderAsset> selectedGeometryShader;
    AssetRef<MaterialAsset> defaultMaterial;
    AssetRef<MaterialAsset> selectedGeometryMaterial;

} // namespace StaticResources

} // namespace Panda