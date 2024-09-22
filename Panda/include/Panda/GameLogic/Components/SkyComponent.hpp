#pragma once

#include "Panda/Assets/AssetLoader.hpp"
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
        : m_sceneViewId(other.m_sceneViewId) {
        initResources();
    }

    SkyComponent()
        : m_sceneViewId(0) {
        initResources();
    }

    ~SkyComponent() {
        Miren::deleteVertexBuffer(m_vertexBuffer);
        Miren::deleteIndexBuffer(m_indexBuffer);
        Miren::deleteProgram(m_shader);
        Miren::deleteTexture(m_skyTexture);
    }

    Miren::TextureHandle getSkyTexture() {
        return m_skyTexture;
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

        Panda::TextureAsset m_skyTextureAsset = AssetLoader::loadCubeMapTexture({
            "default-textures/skybox/2/px.png",
            "default-textures/skybox/2/nx.png",
            "default-textures/skybox/2/py.png",
            "default-textures/skybox/2/ny.png",
            "default-textures/skybox/2/pz.png",
            "default-textures/skybox/2/nz.png",
        });
        Miren::TextureCreate m_skyTextureConfig = m_skyTextureAsset.getMirenTextureCreate();
        m_skyTextureConfig.m_minFiltering = NEAREST;
        m_skyTextureConfig.m_magFiltering = LINEAR;
        m_skyTextureConfig.m_wrap = CLAMP;
        m_skyTexture = Miren::createTexture(m_skyTextureConfig);

        Panda::ProgramAsset programAsset = Panda::AssetLoader::loadProgram(
            "default-shaders/sky/sky_vertex.glsl", "default-shaders/sky/sky_fragment.glsl"
        );
        m_shader = Miren::createProgram(programAsset.getMirenProgramCreate());
        m_model = glm::mat4(1.f);
    }

    void update(glm::mat4 &viewProjection) {
        m_viewProjection = viewProjection;
        Miren::setShader(m_shader);
        int samplerCube = 0;
        Miren::setTexture(m_skyTexture, samplerCube);
        Miren::setUniform(m_shader, "skyTexture", &samplerCube, Miren::UniformType::Sampler);
        Miren::setUniform(m_shader, "model", &m_model[0][0], Miren::UniformType::Mat4);
        Miren::setUniform(
            m_shader, "projViewMtx", &m_viewProjection[0][0], Miren::UniformType::Mat4
        );
        Miren::setVertexBuffer(m_vertexBuffer);
        Miren::setIndexBuffer(m_indexBuffer, 0, 36);
        Miren::setState(0);
        Miren::submit(m_sceneViewId);
    }

    void setViewId(Miren::ViewId viewId) {
        m_sceneViewId = viewId;
    }

private:
    glm::mat4 m_model;
    glm::mat4 m_viewProjection;
    Miren::ViewId m_sceneViewId;

    Miren::TextureHandle m_skyTexture;
    Miren::ProgramHandle m_shader;
    Miren::IndexBufferHandle m_indexBuffer;
    Miren::VertexBufferHandle m_vertexBuffer;
};

} // namespace Panda