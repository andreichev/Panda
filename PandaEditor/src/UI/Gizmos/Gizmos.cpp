//
// Created by Michael Andreichev on 06.10.2024.
//

#include "Gizmos.hpp"

#include <Panda/WorldCommands/Impl/EntityTransformCommand.hpp>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

namespace Panda {

Gizmos::Gizmos(Camera *camera, CameraController *cameraController)
    : m_camera(camera)
    , m_world(nullptr)
    , m_cameraController(cameraController) {}

void Gizmos::onImGuiRender(SceneState sceneState, Rect viewportRect) {
    if (sceneState == SceneState::PLAY) { return; }
    if (!m_camera || !m_world) { return; }
    SelectionContext &selectionContext = m_world->getSelectionContext();
    if (selectionContext.empty()) { return; }
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
    glm::mat4 transform = selectionContext.getMedianMatrix();
    glm::mat4 delta = glm::mat4(1.0f);
    if (!ImGuizmo::Manipulate(
            glm::value_ptr(view),
            glm::value_ptr(projection),
            ImGuizmo::OPERATION::TRANSLATE,
            ImGuizmo::LOCAL,
            glm::value_ptr(transform),
            glm::value_ptr(delta)
        )) {
        return;
    }
    /*
     * IF NEED TO DECOMPOSE (TO APPLY ROTATION TO INDIVIDUAL ORIGINS)
     *
    glm::vec3 deltaPosition;
    glm::vec3 deltaRotation;
    glm::vec3 deltaScale;
    ImGuizmo::DecomposeMatrixToComponents(
        glm::value_ptr(delta),
        glm::value_ptr(deltaPosition),
        glm::value_ptr(deltaRotation),
        glm::value_ptr(deltaScale)
    );
    LOG_INFO_EDITOR("%d, %d, %d", deltaPosition.x, deltaPosition.y, deltaPosition.z);
    */
    auto entities = selectionContext.getManipulatingEntities();
    EntityTransformCommand move(entities);
    move.saveBeforeEdit();
    for (auto entity : entities) {
        TransformComponent &transformComponent = entity.getTransform();
        glm::mat4 newTransform = delta * transformComponent.getLocalTransform();
        transformComponent.setTransform(newTransform);
        if (entity.hasComponent<Rigidbody2DComponent>()) { entity.physics2DPropertiesUpdated(); }
    }
    move.saveAfterEdit();
    WorldCommandManager &cmd = m_world->getCommandManger();
    cmd.SAVE(move, false);
    selectionContext.updateValues();
    m_world->setChanged();
}

bool Gizmos::isUsing() {
    return ImGuizmo::IsUsing();
}

} // namespace Panda
