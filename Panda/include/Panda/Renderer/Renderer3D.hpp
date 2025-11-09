//
// Created by Michael Andreichev on 15.09.2023.
//

#pragma once

#include "Panda/GameLogic/Components/TransformComponent.hpp"
#include "Panda/Assets/MeshAsset.hpp"
#include "Panda/GameLogic/Camera.hpp"

#include <Miren/Miren.hpp>

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
    void begin(Miren::ViewId id);
    void submitSky(glm::mat4 &skyViewProj, AssetRef<MeshAsset> skyMesh);
    void submit(glm::mat4 &model, AssetRef<MeshAsset> mesh);
    void end();
    Statistics getStats();
    void setViewProj(glm::mat4 viewProj);

    Renderer3D &operator=(Renderer3D &&other);

private:
    void flush();

    DrawData m_drawData;
    Miren::ViewId m_viewId;
    glm::mat4 m_viewProj;
};

} // namespace Panda
