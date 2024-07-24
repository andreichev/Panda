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
        ImGui::OpenPopup(popup.title);
        imguiDrawPopup(popup);
    }
    if (m_loader.hasOpenedProject()) {
        m_menuBar.onImGuiRender();
        auto pickedSceneState = m_toolbar.onImGuiRender(m_menuBar.m_height, m_sceneState);
        m_dockspace.beginImGuiDockspace(m_toolbar.height + m_menuBar.m_height);
        m_statisticsPanel.onImGuiRender();
        m_viewport.onImGuiRender();
        m_hierarchyPanel.onImGuiRender();
        m_contentBrowser.onImGuiRender();
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
        EditorPopup popup;
        popup.yesAction = [](void *data) {
            auto self = static_cast<EditorLayer *>(data);
            self->saveWorld();
            Application::get()->close();
        };
        popup.noAction = [](void *data) { Application::get()->close(); };
        popup.userData = this;
        popup.title = "Save current world?";
        popup.subtitle = "Do you want to save your changes?";
        popup.yesText = "Save";
        popup.noText = "Not save";
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
        EditorPopup popup;
        popup.yesAction = [](void *data) {
            auto self = static_cast<EditorLayer *>(data);
            self->saveWorld();
            self->m_loader.closeProject();
        };
        popup.noAction = [](void *data) {
            auto self = static_cast<EditorLayer *>(data);
            self->m_loader.closeProject();
        };
        popup.userData = this;
        popup.title = "Save current world?";
        popup.subtitle = "Do you want to save your changes?";
        popup.yesText = "Save";
        popup.noText = "Not save";
        m_popups.emplace_back(popup);
    } else {
        m_loader.closeProject();
    }
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

void EditorLayer::imguiDrawPopup(EditorLayer::EditorPopup &popup) {
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);
    if (ImGui::BeginPopupModal(popup.title, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text(popup.subtitle);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
        if (ImGui::Button(popup.yesText, {ImGui::GetContentRegionAvail().x, 24}) ||
            Input::isKeyPressed(Key::ENTER)) {
            if (popup.yesAction) {
                popup.yesAction(popup.userData);
            }
            m_popups.pop_back();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button(popup.noText, {ImGui::GetContentRegionAvail().x, 24})) {
            if (popup.noAction) {
                popup.noAction(popup.userData);
            }
            m_popups.pop_back();
            ImGui::CloseCurrentPopup();
        }
        ImGui::Separator();
        if (ImGui::Button("Cancel", {ImGui::GetContentRegionAvail().x, 24})) {
            m_popups.pop_back();
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleVar();
        ImGui::SetItemDefaultFocus();
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
}

void EditorLayer::saveWorld() {
    m_loader.saveWorld();
}

} // namespace Panda
