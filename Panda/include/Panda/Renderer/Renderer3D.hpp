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

    static void init();
    static void begin();
    static void submit(Transform *transform, StaticMesh *mesh);
    static void submit(Transform *transform, DynamicMesh *mesh);
    static void end();
    static Statistics getStats();
    static void terminate();
    static void setCamera(Camera *camera);
    static void setViewId(Miren::ViewId id);

private:
    static Miren::ViewId s_viewId;
    static Camera *s_camera;
};

} // namespace Panda
