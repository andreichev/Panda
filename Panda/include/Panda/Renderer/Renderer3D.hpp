//
// Created by Michael Andreichev on 15.09.2023.
//

#pragma once

#include "Panda/GameLogic/Components/TransformComponent.hpp"
#include "Panda/GameLogic/Components/StaticMesh.hpp"
#include "Panda/GameLogic/Components/DynamicMesh.hpp"
#include "Panda/GameLogic/Camera.hpp"

namespace Panda {

// TODO: - 1. Вычислять матрицу модели только один раз для статических мешей
// TODO: - 2. Batch rendering

class Renderer3D final {
public:
    struct Statistics {
        uint32_t drawCalls = 0;
    };

    struct DrawData {
        Renderer3D::Statistics stats;
    };

    Renderer3D();
    Renderer3D(Renderer3D &&other);
    ~Renderer3D() = default;
    void begin();
    void submit(glm::mat4 &transform, StaticMesh *mesh);
    void submit(glm::mat4 &transform, DynamicMesh *mesh);
    void end();
    Statistics getStats();
    void setViewProj(glm::mat4 viewProj);
    void setViewId(Miren::ViewId id);

    Renderer3D &operator=(Renderer3D &&other);

private:
    DrawData m_drawData;
    Miren::ViewId m_viewId;
    glm::mat4 m_viewProj;
};

} // namespace Panda
