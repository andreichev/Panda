#pragma once

#include "Camera/CameraController.hpp"

#include <Panda/Assets/AssetHandler.hpp>
#include <glm/glm.hpp>
#include <Miren/Miren.hpp>

namespace Panda {

class SceneGrid final {
public:
    SceneGrid(SceneGrid &other)
        : m_cameraController(other.m_cameraController)
        , m_sceneViewId(other.m_sceneViewId) {
        initResources();
    }

    SceneGrid(CameraController *cameraController)
        : m_cameraController(cameraController)
        , m_sceneViewId(0) {
        initResources();
    }

    ~SceneGrid() {
        freeResources();
    }

    SceneGrid &operator=(SceneGrid &other) {
        m_sceneViewId = other.m_sceneViewId;
        m_cameraController = other.m_cameraController;
        initResources();
        return *this;
    }

    void freeResources() {
        Miren::deleteVertexBuffer(m_vertexBuffer);
        Miren::deleteIndexBuffer(m_indexBuffer);
        Miren::deleteProgram(m_shader);
    }

    void initResources() {
        using namespace Miren;
        float rightEdge = 1.0f;
        float topEdge = 1.0f;
        float leftEdge = -1.0f;
        float bottomEdge = -1.0f;
        float data[8] = {
            rightEdge, topEdge, leftEdge, topEdge, leftEdge, bottomEdge, rightEdge, bottomEdge
        };
        uint32_t indices[6] = {0, 1, 2, 0, 2, 3};

        Miren::VertexBufferLayoutData layoutData;
        layoutData.pushVec2();
        Miren::VertexLayoutHandle layoutHandle = Miren::createVertexLayout(layoutData);
        Foundation::Memory verticesMemory = Foundation::Memory::copying(data, sizeof(float) * 8);
        Foundation::Memory indicesMemory =
            Foundation::Memory::copying(indices, sizeof(uint32_t) * 6);
        m_vertexBuffer = Miren::createVertexBuffer(verticesMemory, 8 * sizeof(float), layoutHandle);
        m_indexBuffer =
            Miren::createIndexBuffer(indicesMemory, Miren::BufferElementType::UnsignedInt, 6);

        Panda::ProgramData programAsset = Panda::AssetLoaderEditor::loadProgram(
            "editor-shaders/grid_vertex.glsl", "editor-shaders/grid_fragment.glsl"
        );
        m_shader = Miren::createProgram(programAsset.getMirenProgramCreate());
    }

    void update(glm::mat4 &viewProjection) {
        m_viewProjection = viewProjection;
        Miren::setShader(m_shader);
        glm::vec4 pos = glm::vec4(m_cameraController->getPosition(), 1.0);
        Miren::setUniform(m_shader, "gCameraWorldPos", &pos, Miren::UniformType::Vec4);
        Miren::setUniform(
            m_shader, "projViewMtx", &m_viewProjection[0][0], Miren::UniformType::Mat4
        );
        Miren::setVertexBuffer(m_vertexBuffer);
        Miren::setIndexBuffer(m_indexBuffer, 0, 6);
        Miren::setState(MIREN_STATE_DEPTH_TEST);
        Miren::submit(m_sceneViewId);
    }

    void setViewId(Miren::ViewId viewId) {
        m_sceneViewId = viewId;
    }

private:
    glm::mat4 m_viewProjection;
    Miren::ViewId m_sceneViewId;

    Miren::ProgramHandle m_shader;
    Miren::IndexBufferHandle m_indexBuffer;
    Miren::VertexBufferHandle m_vertexBuffer;
    CameraController *m_cameraController;
};

} // namespace Panda