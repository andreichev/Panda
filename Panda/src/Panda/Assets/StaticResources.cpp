#include "Panda/Assets/StaticResources.hpp"
#include "Panda/GameLogic/GameContext.hpp"

namespace Panda {

namespace StaticResources {

    struct SkyVertex {
        glm::vec3 pos;

        SkyVertex(float x, float y, float z)
            : pos(x, y, z) {}
    };

    void initStaticResources() {
        UUID ASSET_ID = 1;
        AssetHandler *handler = GameContext::getAssetHandler();
        /// ----------------------------------------
        /// SKY MESH
        /// ----------------------------------------
        Miren::TextureCreate skyHdrTextureConfig =
            AssetImporterBase::load2DTexture("default-textures/skybox/hdr/clouds.png");
        skyHdrTextureConfig.m_minFiltering = Miren::TextureFiltering::NEAREST;
        skyHdrTextureConfig.m_magFiltering = Miren::TextureFiltering::NEAREST;
        skyHdrTextureConfig.m_wrap = Miren::TextureWrapMode::CLAMP;
        AssetRef<TextureAsset> defaultSkyTexture =
            handler->createStaticAsset<TextureAsset>(ASSET_ID++, skyHdrTextureConfig);
        AssetRef<ShaderAsset> defaultSkyShader = handler->createStaticAsset<ShaderAsset>(
            ASSET_ID++, "default-shaders/sky/sky_hdr.vert", "default-shaders/sky/sky_hdr.frag"
        );
        MaterialData skyMaterialData;
        skyMaterialData.inputs = {
            MaterialField("skyTexture", MaterialFieldType::TEXTURE, defaultSkyTexture.getId())
        };
        AssetRef<MaterialAsset> skyMaterial = handler->createStaticAsset<MaterialAsset>(
            ASSET_ID++, skyMaterialData, defaultSkyShader
        );
        SkyVertex vertices[24] = {
            // Front
            SkyVertex(-1.0f, -1.0f, 1.0f), // 0
            SkyVertex(1.0f, -1.0f, 1.0f),  // 1
            SkyVertex(1.0f, 1.0f, 1.0f),   // 2
            SkyVertex(-1.0f, 1.0f, 1.0f),  // 3
            // Back
            SkyVertex(-1.0f, -1.0f, -1.0f), // 4
            SkyVertex(-1.0f, 1.0f, -1.0f),  // 5
            SkyVertex(1.0f, 1.0f, -1.0f),   // 6
            SkyVertex(1.0f, -1.0f, -1.0f),  // 7
            // Top
            SkyVertex(-1.0f, 1.0f, -1.0f), // 8
            SkyVertex(-1.0f, 1.0f, 1.0f),  // 11
            SkyVertex(1.0f, 1.0f, 1.0f),   // 10
            SkyVertex(1.0f, 1.0f, -1.0f),  // 9
            // Bottom
            SkyVertex(-1.0f, -1.0f, -1.0f), // 12
            SkyVertex(1.0f, -1.0f, -1.0f),  // 13
            SkyVertex(1.0f, -1.0f, 1.0f),   // 14
            SkyVertex(-1.0f, -1.0f, 1.0f),  // 15
            // Left
            SkyVertex(-1.0f, -1.0f, -1.0f), // 16
            SkyVertex(-1.0f, -1.0f, 1.0f),  // 17
            SkyVertex(-1.0f, 1.0f, 1.0f),   // 18
            SkyVertex(-1.0f, 1.0f, -1.0f),  // 19
            // Right
            SkyVertex(1.0f, -1.0f, -1.0f), // 20
            SkyVertex(1.0f, 1.0f, -1.0f),  // 23
            SkyVertex(1.0f, 1.0f, 1.0f),   // 22
            SkyVertex(1.0f, -1.0f, 1.0f)   // 21
        };
        // clang-format off
        uint32_t indices[36] = {
            0, 2, 1, 2, 0, 3,       // Front
            4, 6, 5, 6, 4, 7,       // Back
            8, 10, 9, 10, 8, 11,    // Top
            12, 14, 13, 14, 12, 15, // Bottom
            16, 18, 17, 18, 16, 19, // Left
            20, 22, 21, 22, 20, 23  // Right
        };
        // clang-format on
        Miren::VertexBufferLayoutData layoutData;
        layoutData.pushVec3();
        Miren::VertexLayoutHandle layoutHandle = Miren::createVertexLayout(layoutData);
        Foundation::Memory verticesMemory =
            Foundation::Memory::copying(vertices, sizeof(SkyVertex) * 24);
        Foundation::Memory indicesMemory =
            Foundation::Memory::copying(indices, sizeof(uint32_t) * 36);
        MeshData skyMeshData(
            layoutHandle, verticesMemory, 24 * sizeof(SkyVertex), indicesMemory, 36
        );
        defaultSkyMesh = handler->createStaticAsset<MeshAsset>(ASSET_ID++);
        defaultSkyMesh->create(skyMeshData, skyMaterial);
        /// ----------------------------------------
        /// WHITE TEXTURE
        /// ----------------------------------------
        Foundation::Memory whiteTextureData = Foundation::Memory::alloc(sizeof(uint32_t));
        *(uint32_t *)whiteTextureData.data = 0xffffffff;
        Miren::TextureCreate whiteTextureCreate;
        whiteTextureCreate.m_data = whiteTextureData;
        whiteTexture = handler->createStaticAsset<TextureAsset>(ASSET_ID++, whiteTextureCreate);
        /// ----------------------------------------
        /// DEFAULT SHADER
        /// ----------------------------------------
        defaultShader =
            handler->createStaticAsset<ShaderAsset>(ASSET_ID++, "default-shaders/default.frag");
        /// ----------------------------------------
        /// DEFAULT MATERIAL
        /// ----------------------------------------
        MaterialData materialData;
        materialData.inputs = {
            MaterialField("albedo", MaterialFieldType::TEXTURE, whiteTexture.getId())
        };
        defaultMaterial =
            handler->createStaticAsset<MaterialAsset>(ASSET_ID++, materialData, defaultShader);
        /// ----------------------------------------
        /// SELECTED GEOMETRY SHADER
        /// ----------------------------------------
        selectedGeometryShader = handler->createStaticAsset<ShaderAsset>(
            ASSET_ID++, "default-shaders/selection_map.frag"
        );
        /// ----------------------------------------
        /// SELECTED GEOMETRY MATERIAL
        /// ----------------------------------------
        selectedGeometryMaterial = handler->createStaticAsset<MaterialAsset>(
            ASSET_ID++, materialData, selectedGeometryShader
        );
    }

    void deinitStaticResources() {
        defaultSkyMesh.reset();
        whiteTexture.reset();
        defaultShader.reset();
        selectedGeometryShader.reset();
        defaultMaterial.reset();
        selectedGeometryMaterial.reset();
    }

    AssetRef<MeshAsset> defaultSkyMesh;
    AssetRef<TextureAsset> whiteTexture;
    AssetRef<ShaderAsset> defaultShader;
    AssetRef<ShaderAsset> selectedGeometryShader;
    AssetRef<MaterialAsset> defaultMaterial;
    AssetRef<MaterialAsset> selectedGeometryMaterial;

} // namespace StaticResources

} // namespace Panda