//
// Created by Michael Andreichev on 15.09.2023.
//

#pragma once

#include "Panda/GameLogic/Components/Transform.hpp"
#include "Panda/GameLogic/Components/StaticMesh.hpp"
#include "Panda/GameLogic/Components/DynamicMesh.hpp"

namespace Panda {

// TODO: - 1. Вычислять матрицу модели только один раз для статических мешей
// TODO: - 2. Batch rendering

class Renderer3D final {
public:
    static void submit(Transform *transform, StaticMesh *mesh);
    static void submit(Transform *transform, DynamicMesh *mesh);
    static void setViewId(Miren::ViewId id);

private:
    static Miren::ViewId s_viewId;
};

} // namespace Panda
