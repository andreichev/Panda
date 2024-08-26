#include "EditorLayer.hpp"

#include "Panels/Common/ImGuiHelper.hpp"
#include "Panels/Popups/EditorYesNoPopup.hpp"
#include "Panels/Popups/PickScriptPopup.hpp"

namespace Panda {

EditorLayer::EditorLayer()
    : m_world()
    , m_toolbar()
    , m_dockspace()
    , m_viewport()
    , m_hierarchyPanel(nullptr, this)
    , m_statisticsPanel(nullptr)
    , m_consolePanel()
    , m_loader(&m_world, this)
    , m_startPanel(&m_loader)
    , m_menuBar(this)
    , m_contentBrowser()
    , m_editorCamera()
    , m_popups()
    , m_cameraController()
    , m_sceneState(SceneState::EDIT) {
    m_viewport.init(&m_world);
    m_viewport.setCamera(&m_editorCamera);
    m_hierarchyPanel.setWorld(&m_world);
    m_statisticsPanel.setWorld(&m_world);
    m_cameraController.setPosition({0.f, 0.f, 4.f});
}

void EditorLayer::onAttach() {
    m_loader.loadInitialData();
}

void EditorLayer::onDetach() {}

void EditorLayer::onUpdate(double deltaTime) {
    if (!m_loader.hasOpenedProject()) {
        return;
    }
    switch (m_sceneState) {
        case SceneState::EDIT: {
            m_cameraController.update(deltaTime);
            glm::mat4 view = m_cameraController.getViewMatrix();
            glm::mat4 proj = m_editorCamera.getProjection();
            glm::mat4 cameraViewProj = proj * m_cameraController.getSkyViewMatrix();
            glm::mat4 viewProj = proj * view;
            m_world.updateEditor(deltaTime, viewProj, cameraViewProj);
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
            glm::mat4 cameraViewProj = proj * m_cameraController.getSkyViewMatrix();
            glm::mat4 viewProj = proj * view;
            m_world.updateSimulation(deltaTime, viewProj, cameraViewProj);
            break;
        }
    }
}

void EditorLayer::onImGuiRender() {
    if (!m_popups.empty()) {
        auto &popup = m_popups.back();
        popup->onImGuiRender();
    }
    if (m_loader.hasOpenedProject()) {
        m_menuBar.onImGuiRender();
        auto pickedSceneState = m_toolbar.onImGuiRender(m_menuBar.m_height, m_sceneState);
        m_dockspace.beginImGuiDockspace(m_toolbar.height + m_menuBar.m_height);
        m_statisticsPanel.onImGuiRender();
        m_viewport.onImGuiRender();
        m_hierarchyPanel.onImGuiRender();
        m_contentBrowser.onImGuiRender();
        m_consolePanel.onImGuiRender();
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
    m_world.setRunning(true);
    m_viewport.focus();
}

void EditorLayer::simulate() {
    m_sceneState = SceneState::SIMULATE;
    m_world.setRunning(true);
    m_viewport.focus();
}

void EditorLayer::stop() {
    m_sceneState = SceneState::EDIT;
    m_viewport.setCamera(&m_editorCamera);
    m_world.setRunning(false);
    m_viewport.focus();
}

#pragma region Project loader output

void EditorLayer::loaderDidLoadProject(const path_t &path) {
    updateWindowState();
    m_contentBrowser.setBaseDirectory(path);
}

void EditorLayer::loaderDidLoadWorld() {
    Application::get()->getWindow()->setTitle(m_loader.getProjectSettings().worldPath.c_str());
}

void EditorLayer::loaderDidLoadCloseProject() {
    m_world.clear();
    updateWindowState();
}

void EditorLayer::loaderCreateSampleWorld() {
    Application::get()->getWindow()->setTitle("Untitled World");
    m_world.clear();
    m_world.fillStartupData();
}

#pragma endregion

#pragma region Menu bar output

void EditorLayer::menuBarOpenProject() {
    std::optional<path_t> optionalPath = SystemTools::openFolderDialog();
    if (!optionalPath.has_value()) {
        return;
    }
    path_t path = optionalPath.value();
    m_loader.openProject(path);
}

void EditorLayer::menuBarCloseApp() {
    if (m_world.isChanged()) {
        EditorYesNoPopup *popup = F_NEW(Foundation::getAllocator(), EditorYesNoPopup);
        popup->yesAction = [](void *data) {
            auto self = static_cast<EditorLayer *>(data);
            self->saveWorld();
            Application::get()->close();
            F_DELETE(Foundation::getAllocator(), self->m_popups.back());
            self->m_popups.pop_back();
        };
        popup->noAction = [](void *data) {
            Application::get()->close();
            auto self = static_cast<EditorLayer *>(data);
            F_DELETE(Foundation::getAllocator(), self->m_popups.back());
            self->m_popups.pop_back();
        };
        popup->closeAction = [](void *data) {
            auto self = static_cast<EditorLayer *>(data);
            F_DELETE(Foundation::getAllocator(), self->m_popups.back());
            self->m_popups.pop_back();
        };
        popup->userData = this;
        popup->title = "Save current world?";
        popup->subtitle = "Do you want to save your changes?";
        popup->yesText = "Save";
        popup->noText = "Not save";
        m_popups.emplace_back(popup);
    } else {
        Application::get()->close();
    }
}

void EditorLayer::menuBarSaveWorld() {
    saveWorld();
}

void EditorLayer::menuBarCloseProject() {
    if (m_world.isChanged()) {
        EditorYesNoPopup *popup = F_NEW(Foundation::getAllocator(), EditorYesNoPopup);
        popup->yesAction = [](void *data) {
            auto self = static_cast<EditorLayer *>(data);
            self->saveWorld();
            self->m_loader.closeProject();
            F_DELETE(Foundation::getAllocator(), self->m_popups.back());
            self->m_popups.pop_back();
        };
        popup->noAction = [](void *data) {
            auto self = static_cast<EditorLayer *>(data);
            self->m_loader.closeProject();
            F_DELETE(Foundation::getAllocator(), self->m_popups.back());
            self->m_popups.pop_back();
        };
        popup->closeAction = [](void *data) {
            auto self = static_cast<EditorLayer *>(data);
            F_DELETE(Foundation::getAllocator(), self->m_popups.back());
            self->m_popups.pop_back();
        };
        popup->userData = this;
        popup->title = "Save current world?";
        popup->subtitle = "Do you want to save your changes?";
        popup->yesText = "Save";
        popup->noText = "Not save";
        m_popups.emplace_back(popup);
    } else {
        m_loader.closeProject();
    }
}

#pragma endregion

#pragma region Components draw output

void EditorLayer::addScriptToEntity(Entity entity) {
    PickScriptPopup *popup = F_NEW(Foundation::getAllocator(), PickScriptPopup);
    popup->closeAction = [](void *data) {
        auto self = static_cast<EditorLayer *>(data);
        F_DELETE(Foundation::getAllocator(), self->m_popups.back());
        self->m_popups.pop_back();
    };
    popup->selectAction = [](void *data, Entity entity, const char *scriptName) {
        auto self = static_cast<EditorLayer *>(data);
        if (scriptName) {
            ScriptHandle id = ExternalCalls::addScriptFunc(scriptName);
            if (id) {
                entity.addScript(Panda::ExternalScript(id, scriptName));
            }
        }
        F_DELETE(Foundation::getAllocator(), self->m_popups.back());
        self->m_popups.pop_back();
    };
    popup->userData = this;
    popup->entity = entity;
    popup->title = "Add Script";
    popup->subtitle = "Pick a script from available Script classes list.";
    m_popups.emplace_back(popup);
}

#pragma endregion

void EditorLayer::updateWindowState() {
    auto *window = Application::get()->getWindow();
    window->setResizable(m_loader.hasOpenedProject());
    window->setMaximized(m_loader.hasOpenedProject());
    if (!m_loader.hasOpenedProject()) {
        Application::get()->getWindow()->setTitle("Welcome");
        window->setSize({600, 400});
    }
}

void EditorLayer::saveWorld() {
    m_loader.saveWorld();
}

} // namespace Panda
