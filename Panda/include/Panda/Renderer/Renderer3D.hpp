//
// Created by Michael Andreichev on 15.09.2023.
//

#pragma once

#include "Panda/GameLogic/Components/Transform.hpp"
#include "Panda/GameLogic/Components/StaticMesh.hpp"
#include "Panda/GameLogic/Components/DynamicMesh.hpp"
#include "Panda/GameLogic/Components/Camera.hpp"

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
    ~Renderer3D();
    void begin();
    void submit(Transform *transform, StaticMesh *mesh);
    void submit(Transform *transform, DynamicMesh *mesh);
    void end();
    Statistics getStats();
    void setCamera(Camera *camera);
    void setViewId(Miren::ViewId id);

private:
    DrawData m_drawData;
    Miren::ViewId m_viewId;
    Camera *m_camera;
};

} // namespace Panda
