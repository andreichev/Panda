#include "EditorLayer.hpp"
#include "Panels/Dockspace.hpp"
#include "Panels/MenuBar.hpp"

#include "Camera/CameraController.hpp"

namespace Panda {

EditorLayer::EditorLayer()
    : m_world(nullptr)
    , m_viewport()
    , m_hierarchyPanel(nullptr)
    , m_statisticsPanel(nullptr)
    , m_editorCamera()
    , m_cameraController()
    , m_sceneState(SceneState::EDIT) {}

void EditorLayer::onAttach() {
    m_world = NEW(Foundation::getAllocator(), World);
    m_world->initialize();
    m_viewport.init(m_world);
    m_viewport.setCamera(&m_editorCamera);
    m_hierarchyPanel.setWorld(m_world);
    m_statisticsPanel.setWorld(m_world);
    initializeEmptyWorld();
    m_cameraController.setPosition({0.f, 0.f, 4.f});
}

void EditorLayer::initializeEmptyWorld() {
    Entity cameraEntity = m_world->instantiateEntity();
    cameraEntity.setName("Camera");
    cameraEntity.getTransform().setPosition({0.f, 0.f, 4.f});
    cameraEntity.addComponent<CameraComponent>();

    Entity sprite1Entity = m_world->instantiateEntity();
    sprite1Entity.setName("Orange Sprite");
    auto &sprite1 = sprite1Entity.addComponent<SpriteRendererComponent>();
    sprite1.color = {1.0f, 0.5f, 0.2f, 1.0f};
}

void EditorLayer::onDetach() {
    DELETE(Foundation::getAllocator(), m_world);
}

void EditorLayer::onUpdate(double deltaTime) {
    switch (m_sceneState) {
        case SceneState::EDIT: {
            m_cameraController.update(deltaTime);
            glm::mat4 view = m_cameraController.getViewMatrix();
            glm::mat4 proj = m_editorCamera.getProjection();
            m_world->updateEditor(deltaTime, proj * view);
            break;
        }
        case SceneState::PLAY: {
            m_viewport.setCamera(m_world->getMainCamera());
            m_world->updateRuntime(deltaTime);
            break;
        }
        case SceneState::SIMULATE: {
            // TODO: Implement
            break;
        }
    }
}

void EditorLayer::onImGuiRender() {
    Dockspace::beginImGuiDockspace();
    MenuBar::onImGuiRender();
    m_statisticsPanel.onImGuiRender();
    m_viewport.onImGuiRender();
    m_hierarchyPanel.onImGuiRender();
    Dockspace::endImGuiDockspace();
}

void EditorLayer::onEvent(Event *event) {
    Input::onEvent(event);
}

void EditorLayer::play() {
    m_sceneState = SceneState::PLAY;
}

void EditorLayer::stop() {
    m_sceneState = SceneState::EDIT;
    m_viewport.setCamera(&m_editorCamera);
}

} // namespace Panda
