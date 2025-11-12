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
    if (SelectionContext::isEmpty()) { return; }
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
    glm::mat4 transform = SelectionContext::getMedianMatrix();
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
    LOG_INFO_EDITOR("%.3f, %.3f, %.3f", deltaPosition.x, deltaPosition.y, deltaPosition.z);
     */

    auto ids = SelectionContext::getManipulatingEntities();
    std::unordered_set<Entity> entities = m_world->getById(ids);
    EntityTransformCommand move(entities);
    move.saveBeforeEdit();
    for (auto entity : entities) {
        TransformComponent &transformComponent = entity.getTransform();
        glm::mat4 newWorldTransform = delta * m_world->getWorldSpaceTransformMatrix(entity);
        transformComponent.setTransform(newWorldTransform);
        m_world->convertToLocalSpace(entity);
        if (entity.hasComponent<Rigidbody2DComponent>()) { entity.physics2DPropertiesUpdated(); }
    }
    move.saveAfterEdit();
    WorldCommandManager &cmd = m_world->getCommandManger();
    cmd.SAVE(move, false);
    SelectionContext::updateValues();
    m_world->setChanged();
}

bool Gizmos::isUsing() {
    return ImGuizmo::IsUsing();
}

} // namespace Panda
