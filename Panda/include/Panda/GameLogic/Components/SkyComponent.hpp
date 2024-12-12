#pragma once

#include "Panda/Assets/AssetHandler.hpp"
#include "Panda/Application/Application.hpp"

#include <glm/glm.hpp>
#include <Miren/Miren.hpp>

namespace Panda {

struct SkyVertex {
    glm::vec3 pos;

    SkyVertex(float x, float y, float z)
        : pos(x, y, z) {}
};

class SkyComponent final {
public:
    SkyComponent(SkyComponent &other)
        : m_sceneViewId(other.m_sceneViewId)
        , m_useHdr(false) {
        initResources();
    }

    SkyComponent()
        : m_sceneViewId(0)
        , m_useHdr(false) {
        initResources();
    }

    ~SkyComponent() {
        freeResources();
    }

    SkyComponent &operator=(SkyComponent &other) {
        m_sceneViewId = other.m_sceneViewId;
        initResources();
        return *this;
    }

    Miren::TextureHandle getSkyTexture() {
        return m_skyDefaultTexture;
    }

    Miren::TextureHandle getSkyHdrTexture() {
        return m_skyHdrTexture;
    }

    void freeResources() {
        Miren::deleteVertexBuffer(m_vertexBuffer);
        Miren::deleteIndexBuffer(m_indexBuffer);
        Miren::deleteProgram(m_defaultShader);
        Miren::deleteTexture(m_skyDefaultTexture);
    }

    void initResources() {
        using namespace Miren;
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
            0, 1, 2, 2, 3, 0,       // Front
            4, 5, 6, 6, 7, 4,       // Back
            8, 9, 10, 10, 11, 8,    // Top
            12, 13, 14, 14, 15, 12, // Bottom
            16, 17, 18, 18, 19, 16, // Left
            20, 21, 22, 22, 23, 20  // Right
        };
        // clang-format on

        Miren::VertexBufferLayoutData layoutData;
        layoutData.pushVec3();
        Miren::VertexLayoutHandle layoutHandle = Miren::createVertexLayout(layoutData);
        Foundation::Memory verticesMemory =
            Foundation::Memory::copying(vertices, sizeof(SkyVertex) * 24);
        Foundation::Memory indicesMemory =
            Foundation::Memory::copying(indices, sizeof(uint32_t) * 36);
        m_vertexBuffer =
            Miren::createVertexBuffer(verticesMemory, 24 * sizeof(SkyVertex), layoutHandle);
        m_indexBuffer =
            Miren::createIndexBuffer(indicesMemory, Miren::BufferElementType::UnsignedInt, 36);

        Panda::TextureData m_skyDefaultTextureAsset = AssetLoaderEditor::loadCubeMapTexture({
            "default-textures/skybox/3/px.png",
            "default-textures/skybox/3/nx.png",
            "default-textures/skybox/3/py.png",
            "default-textures/skybox/3/ny.png",
            "default-textures/skybox/3/pz.png",
            "default-textures/skybox/3/nz.png",
        });
        Miren::TextureCreate m_skyDefaultTextureConfig = m_skyDefaultTextureAsset.getMirenTextureCreate();
        m_skyDefaultTextureConfig.m_minFiltering = NEAREST;
        m_skyDefaultTextureConfig.m_magFiltering = LINEAR;
        m_skyDefaultTextureConfig.m_wrap = CLAMP;
        m_skyDefaultTexture = Miren::createTexture(m_skyDefaultTextureConfig);

        Panda::ProgramData programAsset = Panda::AssetLoaderEditor::loadProgram(
            "default-shaders/sky/sky_vertex.glsl", "default-shaders/sky/sky_fragment.glsl"
        );
        m_defaultShader = Miren::createProgram(programAsset.getMirenProgramCreate());
        
        programAsset = Panda::AssetLoaderEditor::loadProgram(
            "default-shaders/sky/sky_vertex_hdr.glsl", "default-shaders/sky/sky_fragment_hdr.glsl"
        );
        m_hdrShader = Miren::createProgram(programAsset.getMirenProgramCreate());
        m_model = glm::mat4(1.f);

        setHdrTexture("default-textures/skybox/hdr/field.jpeg");
    }

    void update(glm::mat4 &viewProjection) {
        m_viewProjection = viewProjection;
        auto &shader = m_useHdr ? m_hdrShader : m_defaultShader;
        Miren::setShader(shader);
        int samplerCube = 0;
        Miren::setTexture(m_useHdr ? m_skyHdrTexture : m_skyDefaultTexture, samplerCube);
        Miren::setUniform(shader, "skyTexture", &samplerCube, Miren::UniformType::Sampler);
        Miren::setUniform(shader, "model", &m_model[0][0], Miren::UniformType::Mat4);
        Miren::setUniform(
            shader, "projViewMtx", &m_viewProjection[0][0], Miren::UniformType::Mat4
        );
        Miren::setVertexBuffer(m_vertexBuffer);
        Miren::setIndexBuffer(m_indexBuffer, 0, 36);
        Miren::setState(0);
        Miren::submit(m_sceneViewId);
    }

    void setHdrTexture(const path_t &pathHdrTexture) {
        using namespace Miren;
        if (!std::filesystem::exists(pathHdrTexture)) {
            LOG_ERROR("Path not exists");
            return;
        }
        deleteHdrTexture();
        m_useHdr = true;
        
        Panda::TextureData m_skyDefaultTextureAsset = AssetLoaderEditor::loadTexture(pathHdrTexture);
        Miren::TextureCreate m_skyHdrTextureConfig = m_skyDefaultTextureAsset.getMirenTextureCreate();
        m_skyHdrTextureConfig.m_minFiltering = NEAREST;
        m_skyHdrTextureConfig.m_magFiltering = LINEAR;
        m_skyHdrTextureConfig.m_wrap = CLAMP;
        m_skyHdrTexture = Miren::createTexture(m_skyHdrTextureConfig);
    }
    
    void deleteHdrTexture() {
        if (m_skyHdrTexture.isValid()) {
            Miren::deleteTexture(m_skyHdrTexture);
        }
        m_useHdr = false;
    }

    void setViewId(Miren::ViewId viewId) {
        m_sceneViewId = viewId;
    }

private:
    bool m_useHdr;
    glm::mat4 m_model;
    glm::mat4 m_viewProjection;
    Miren::ViewId m_sceneViewId;

    Miren::TextureHandle m_skyDefaultTexture;
    Miren::TextureHandle m_skyHdrTexture;
    Miren::ProgramHandle m_defaultShader;
    Miren::ProgramHandle m_hdrShader;
    
    Miren::IndexBufferHandle m_indexBuffer;
    Miren::VertexBufferHandle m_vertexBuffer;
};

} // namespace Panda