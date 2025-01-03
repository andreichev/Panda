//
// Created by Michael Andreichev on 06.10.2024.
//

#include "Gizmos.hpp"

#include <Panda/GameLogic/WorldCommands/Impl/EntityTransformCommand.hpp>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

namespace Panda {

Gizmos::Gizmos(Camera *camera, CameraController *cameraController)
    : m_camera(camera)
    , m_world(nullptr)
    , m_cameraController(cameraController) {}

void Gizmos::onImGuiRender(SceneState sceneState, Rect viewportRect) {
    if (sceneState == SceneState::PLAY) {
        return;
    }
    if (!m_camera || !m_world) {
        return;
    }
    Entity selected = m_world->getSelectedEntity();
    if (!selected.isValid()) {
        return;
    }
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();

    ImGuizmo::SetRect(
        viewportRect.origin.x,
        viewportRect.origin.y,
        viewportRect.size.width,
        viewportRect.size.height
    );

    glm::mat4 view = m_cameraController->getViewMatrix();
    glm::mat4 projection = m_camera->getProjection();
    TransformComponent &transformComponent = selected.getTransform();
    glm::mat4 transform = transformComponent.getTransform();

    ImGuizmo::Manipulate(
        glm::value_ptr(view),
        glm::value_ptr(projection),
        ImGuizmo::OPERATION::TRANSLATE,
        ImGuizmo::LOCAL,
        glm::value_ptr(transform)
    );

    if (isUsing()) {
        glm::vec3 pos = glm::vec3(transform[3]);
        if (m_world->isRunning()) {
            transformComponent.setPosition(pos);
            if (selected.hasComponent<Rigidbody2DComponent>()) {
                selected.physics2DPropertiesUpdated();
            }
        } else {
            TransformComponent transformCopy = transformComponent;
            transformCopy.setPosition(pos);
            EntityTransformCommand move(selected, transformCopy);
            WorldCommandManager &cmd = m_world->getCommandManger();
            cmd.SAVE(move);
        }
    }
}

bool Gizmos::isUsing() {
    return ImGuizmo::IsUsing();
}

} // namespace Panda
