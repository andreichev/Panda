#include "EditorLayer.hpp"

#include "Panels/Common/ImGuiHelper.hpp"

namespace Panda {

EditorLayer::EditorLayer()
    : m_world()
    , m_toolbar()
    , m_dockspace()
    , m_viewport()
    , m_hierarchyPanel(nullptr)
    , m_statisticsPanel(nullptr)
    , m_loader()
    , m_startPanel(&m_loader)
    , m_editorCamera()
    , m_cameraController()
    , m_sceneState(SceneState::EDIT) {}

void EditorLayer::onAttach() {
    auto *window = Application::get()->getWindow();
    window->setResizable(m_loader.hasOpenedProject());
    window->setMaximized(m_loader.hasOpenedProject());
    if (!m_loader.hasOpenedProject()) {
        window->setSize({600, 400});
    }
}

void EditorLayer::setWorld(Panda::World *world) {
    m_world = *world;
    m_viewport.init(&m_world);
    m_viewport.setCamera(&m_editorCamera);
    m_hierarchyPanel.setWorld(&m_world);
    m_statisticsPanel.setWorld(&m_world);
    m_cameraController.setPosition({0.f, 0.f, 4.f});
}

void EditorLayer::initializeEmptyWorld() {
    Entity cameraEntity = m_world.instantiateEntity();
    cameraEntity.setName("Camera");
    cameraEntity.getTransform().setPosition({0.f, 0.f, 4.f});
    cameraEntity.addComponent<CameraComponent>();

    Entity sprite1Entity = m_world.instantiateEntity();
    sprite1Entity.setName("Orange Sprite");
    auto &sprite1 = sprite1Entity.addComponent<SpriteRendererComponent>();
    sprite1.color = {1.0f, 0.5f, 0.2f, 1.0f};
}

void EditorLayer::onDetach() {}

void EditorLayer::onUpdate(double deltaTime) {
    if (m_world.isEmpty()) {
        return;
    }
    switch (m_sceneState) {
        case SceneState::EDIT: {
            m_cameraController.update(deltaTime);
            glm::mat4 view = m_cameraController.getViewMatrix();
            glm::mat4 proj = m_editorCamera.getProjection();
            m_world.updateEditor(deltaTime, proj * view);
            break;
        }
        case SceneState::PLAY: {
            m_viewport.setCamera(m_world.findMainCamera());
            m_world.updateRuntime(deltaTime);
            break;
        }
        case SceneState::SIMULATE: {
            m_cameraController.update(deltaTime);
            glm::mat4 view = m_cameraController.getViewMatrix();
            glm::mat4 proj = m_editorCamera.getProjection();
            m_world.updateSimulation(deltaTime, proj * view);
            break;
        }
    }
}

void EditorLayer::onImGuiRender() {
    if (m_loader.hasOpenedWorld()) {
        m_menuBar.onImGuiRender();
        SceneState pickedSceneState = m_toolbar.onImGuiRender(m_menuBar.height, m_sceneState);
        m_dockspace.beginImGuiDockspace(m_toolbar.height + m_menuBar.height);
        m_statisticsPanel.onImGuiRender();
        m_viewport.onImGuiRender();
        m_hierarchyPanel.onImGuiRender();
        m_dockspace.endImGuiDockspace();

        if (pickedSceneState != m_sceneState) {
            switch (pickedSceneState) {
                case SceneState::EDIT: {
                    stop();
                    break;
                }
                case SceneState::PLAY: {
                    play();
                    break;
                }
                case SceneState::SIMULATE:
                    simulate();
                    break;
            }
            m_sceneState = pickedSceneState;
        }
        m_cameraController.setActive(m_viewport.isFocused());
    } else {
        m_startPanel.onImGuiRender();
    }
}

void EditorLayer::onEvent(Event *event) {
    Input::onEvent(event);
}

void EditorLayer::play() {
    m_sceneState = SceneState::PLAY;
    m_viewport.focus();
}

void EditorLayer::simulate() {
    m_sceneState = SceneState::SIMULATE;
    m_viewport.focus();
}

void EditorLayer::stop() {
    m_sceneState = SceneState::EDIT;
    m_viewport.setCamera(&m_editorCamera);
    m_viewport.focus();
}

} // namespace Panda
