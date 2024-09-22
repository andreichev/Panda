#include "EditorLayer.hpp"

#include "Panda/GameLogic/GameContext.hpp"
#include "Panels/Common/ImGuiHelper.hpp"
#include "Panels/Popups/EditorYesNoPopup.hpp"
#include "Panels/Popups/PickScriptPopup.hpp"
#include "Panels/Popups/EnterNamePopup.hpp"

namespace Panda {

EditorLayer::EditorLayer()
    : m_editingWorld()
    , m_playingWorld()
    , m_currentWorld(&m_editingWorld)
    , m_toolbar(this)
    , m_dockspace()
    , m_viewport()
    , m_hierarchyPanel(nullptr, this)
    , m_statisticsPanel(nullptr)
    , m_consolePanel()
    , m_loader(&m_editingWorld, this)
    , m_startPanel(&m_loader)
    , m_menuBar(this)
    , m_contentBrowser(this)
    , m_editorCamera()
    , m_popups()
    , m_cameraController()
    , m_sceneState(SceneState::EDIT) {
    m_viewport.init();
    m_editingWorld.setViewId(m_viewport.getViewId());
    m_playingWorld.setViewId(m_viewport.getViewId());
    m_viewport.setCamera(&m_editorCamera);
    m_hierarchyPanel.setWorld(m_currentWorld);
    m_statisticsPanel.setWorld(m_currentWorld);
    m_cameraController.setPosition({0.f, 0.f, 4.f});
}

void EditorLayer::onAttach() {
    Foundation::EditorLogger::init(ConsolePanel::loggerCallback);
    m_loader.loadInitialData();
    GameContext::s_currentWorld = m_currentWorld;
    LOG_EDITOR("EDITOR INITIALIZED");
}

void EditorLayer::onDetach() {
    GameContext::s_currentWorld = nullptr;
}

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
            m_editingWorld.updateEditor(deltaTime, viewProj, cameraViewProj);
            break;
        }
        case SceneState::PLAY: {
            m_viewport.setCamera(m_playingWorld.findMainCamera());
            m_playingWorld.updateRuntime(deltaTime);
            break;
        }
        case SceneState::SIMULATE: {
            m_cameraController.update(deltaTime);
            glm::mat4 view = m_cameraController.getViewMatrix();
            glm::mat4 proj = m_editorCamera.getProjection();
            glm::mat4 cameraViewProj = proj * m_cameraController.getSkyViewMatrix();
            glm::mat4 viewProj = proj * view;
            m_playingWorld.updateSimulation(deltaTime, viewProj, cameraViewProj);
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
        m_toolbar.onImGuiRender(m_menuBar.m_height);
        m_dockspace.beginImGuiDockspace(m_toolbar.getHeight() + m_menuBar.m_height + 2);
        m_statisticsPanel.onImGuiRender();
        m_viewport.onImGuiRender();
        m_hierarchyPanel.onImGuiRender();
        m_contentBrowser.onImGuiRender();
        m_consolePanel.onImGuiRender();
        m_dockspace.endImGuiDockspace();
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
    m_playingWorld = m_editingWorld;
    m_currentWorld = &m_playingWorld;
    GameContext::s_currentWorld = m_currentWorld;
    m_hierarchyPanel.setWorld(m_currentWorld);
    m_statisticsPanel.setWorld(m_currentWorld);
    m_playingWorld.startRunning();
    m_viewport.focus();
}

void EditorLayer::simulate() {
    m_sceneState = SceneState::SIMULATE;
    m_currentWorld = &m_playingWorld;
    GameContext::s_currentWorld = m_currentWorld;
    m_hierarchyPanel.setWorld(m_currentWorld);
    m_statisticsPanel.setWorld(m_currentWorld);
    m_playingWorld.startRunning();
    m_viewport.focus();
}

void EditorLayer::stop() {
    m_sceneState = SceneState::EDIT;
    m_viewport.setCamera(&m_editorCamera);
    m_currentWorld = &m_editingWorld;
    GameContext::s_currentWorld = m_currentWorld;
    m_hierarchyPanel.setWorld(m_currentWorld);
    m_statisticsPanel.setWorld(m_currentWorld);
    m_playingWorld.finishRunning();
    m_viewport.focus();
}

#pragma region Project loader output

void EditorLayer::loaderDidLoadProject(const std::string &name, const path_t &path) {
    updateWindowState();
    m_contentBrowser.setBaseDirectory(path / "Assets");
}

void EditorLayer::loaderDidLoadWorld() {
    Application::get()->getWindow()->setTitle(m_loader.getProjectSettings().worldPath.c_str());
}

void EditorLayer::loaderDidLoadCloseProject() {
    m_editingWorld.clear();
    m_playingWorld.clear();
    updateWindowState();
}

void EditorLayer::loaderCreateSampleWorld() {
    Application::get()->getWindow()->setTitle("Untitled World");
    m_editingWorld.clear();
    m_playingWorld.clear();
    m_editingWorld.fillStartupData();
}

EditorCameraSettings EditorLayer::getEditorCameraSettings() {
    EditorCameraSettings settings;
    settings.position = m_cameraController.getPosition();
    settings.rotation = m_cameraController.getRotation();
    return settings;
}

void EditorLayer::setEditorCameraSettings(EditorCameraSettings settings) {
    m_cameraController.setPosition(settings.position);
    m_cameraController.setRotation(settings.rotation);
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

void EditorLayer::menuBarOpenProject(const RecentProject &project) {
    m_loader.openProject(project.path);
}

const std::vector<RecentProject> &EditorLayer::menuBarGetRecentProjectsList() {
    return m_loader.getRecentProjectsList();
}

const path_t &EditorLayer::menuBarGetOpenedProjectPath() {
    return m_loader.getOpenedProjectPath();
}

void EditorLayer::menuBarCloseApp() {
    if (m_editingWorld.isChanged()) {
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

void EditorLayer::menuBarOpenCppProject() {
    m_loader.openCppProject();
}

void EditorLayer::menuBarSaveWorld() {
    saveWorld();
}

void EditorLayer::menuBarCloseProject() {
    if (m_editingWorld.isChanged()) {
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
            ScriptHandle id = ExternalCalls::addScriptFunc(entity.getId(), scriptName);
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

#pragma region Content browser output

void EditorLayer::showCreateFolderPopup() {
    EnterNamePopup *popup = F_NEW(Foundation::getAllocator(), EnterNamePopup);
    popup->closeAction = [](void *data) {
        auto self = static_cast<EditorLayer *>(data);
        F_DELETE(Foundation::getAllocator(), self->m_popups.back());
        self->m_popups.pop_back();
    };
    popup->doneAction = [](void *data, std::string text) {
        auto self = static_cast<EditorLayer *>(data);
        if (!text.empty()) {
            self->m_contentBrowser.createFolder(text);
        }
        F_DELETE(Foundation::getAllocator(), self->m_popups.back());
        self->m_popups.pop_back();
    };
    popup->userData = this;
    popup->title = "Create folder";
    popup->subtitle = "Enter folder name:";
    m_popups.emplace_back(popup);
}

void EditorLayer::deleteFileShowPopup(path_t path) {
    EditorYesNoPopup *popup = F_NEW(Foundation::getAllocator(), EditorYesNoPopup);
    popup->yesAction = [](void *data) {
        auto self = static_cast<EditorLayer *>(data);
        self->m_contentBrowser.confirmDeletion();
        F_DELETE(Foundation::getAllocator(), self->m_popups.back());
        self->m_popups.pop_back();
    };
    popup->noAction = [](void *data) {
        auto self = static_cast<EditorLayer *>(data);
        F_DELETE(Foundation::getAllocator(), self->m_popups.back());
        self->m_popups.pop_back();
    };
    popup->closeAction = popup->noAction;
    popup->userData = this;
    popup->title = "Delete";
    popup->subtitle = "Delete \"" + path.filename().string() + "\"?";
    popup->yesText = "Yes";
    popup->noText = "No";
    m_popups.emplace_back(popup);
}

#pragma endregion

#pragma region Toolbar output

void EditorLayer::toolbarDidPickSceneState(SceneState state) {
    switch (state) {
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
    m_sceneState = state;
}

void EditorLayer::toolbarDidTapReloadScripts() {
    m_loader.reloadScriptsDll();
}

SceneState EditorLayer::toolbarGetCurrentSceneState() {
    return m_sceneState;
}

#pragma endregion

void EditorLayer::updateWindowState() {
    auto *window = Application::get()->getWindow();
    window->setResizable(m_loader.hasOpenedProject());
    window->setMaximized(m_loader.hasOpenedProject());
    if (!m_loader.hasOpenedProject()) {
        window->setTitle("Welcome");
        window->setSize({600, 400});
    }
}

void EditorLayer::saveWorld() {
    switch (m_sceneState) {
        case SceneState::EDIT:
        case SceneState::SIMULATE:
            m_loader.saveWorld();
            break;
        case SceneState::PLAY:
            m_editingWorld = m_playingWorld;
            m_loader.saveWorld();
            break;
    }
}

} // namespace Panda
