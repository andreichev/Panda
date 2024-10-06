//
// Created by Michael Andreichev on 06.10.2024.
//

#pragma once

#include "Camera/EditorCamera.hpp"
#include "Camera/CameraController.hpp"
#include "EditorLayer/SceneState.hpp"

#include <Panda/GameLogic/World.hpp>

namespace Panda {

class Gizmos final {
public:
    Gizmos(Camera *camera, CameraController *cameraController);

    void onImGuiRender(SceneState sceneState, Rect viewportRect);
    void setWorld(World *world) {
        m_world = world;
    }

    void setCamera(Camera *camera) {
        m_camera = camera;
    }

    bool isOver();

private:
    Camera *m_camera;
    CameraController *m_cameraController;
    World *m_world;
};

} // namespace Panda
