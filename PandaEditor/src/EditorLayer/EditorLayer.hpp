#pragma once

#include "Panels/Viewport.hpp"
#include "Panels/MenuBar.hpp"
#include "Panels/Dockspace.hpp"
#include "Panels/WorldHierarchyPanel.hpp"
#include "Panels/StatisticsPanel.hpp"
#include "Panels/Console/ConsolePanel.hpp"
#include "Panels/Toolbar/Toolbar.hpp"
#include "Panels/StartPanel.hpp"
#include "Panels/ContentBrowser/ContentBrowser.hpp"
#include "Camera/EditorCamera.hpp"
#include "Camera/CameraController.hpp"
#include "EditorLayer/SceneState.hpp"
#include "Panels/Popups/EditorPopup.hpp"
#include "ProjectLoader/ProjectLoader.hpp"

#include <Panda.hpp>

namespace Panda {

class EditorLayer : public Layer,
                    public ProjectLoaderOutput,
                    public MenuBarOutput,
                    public ComponentsDrawOutput,
                    public ContentBrowserOutput,
                    public ToolbarOutput {
public:
    EditorLayer();
    ~EditorLayer() override = default;

    virtual void onAttach() override;
    virtual void onDetach() override;

    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onEvent(Event *event) override;

    void play();
    void simulate();
    void stop();

#pragma region Project loader output

    void loaderDidLoadProject(const std::string &name, const path_t &path) override;
    void loaderDidLoadWorld() override;
    void loaderDidLoadCloseProject() override;
    void loaderCreateSampleWorld() override;
    EditorCameraSettings getEditorCameraSettings() override;
    void setEditorCameraSettings(EditorCameraSettings settings) override;

#pragma endregion

#pragma region Menu bar output

    void menuBarOpenProject() override;
    void menuBarOpenProject(const RecentProject &project) override;
    const std::vector<RecentProject> &menuBarGetRecentProjectsList() override;
    const path_t &menuBarGetOpenedProjectPath() override;
    void menuBarCloseApp() override;
    void menuBarSaveWorld() override;
    void menuBarCloseProject() override;

#pragma endregion

#pragma region Components draw output

    void addScriptToEntity(Entity entity) override;

#pragma endregion

#pragma region Content browser output

    void showCreateFolderPopup() override;
    void deleteFileShowPopup(path_t path) override;

#pragma endregion

#pragma region Toolbar output

    void toolbarDidPickSceneState(SceneState state) override;
    void toolbarDidTapReloadScripts() override;
    SceneState toolbarGetCurrentSceneState() override;

#pragma endregion

private:
    void saveWorld();
    void updateWindowState();

    ProjectLoader m_loader;
    MenuBar m_menuBar;
    Toolbar m_toolbar;
    Dockspace m_dockspace;
    Viewport m_viewport;
    StatisticsPanel m_statisticsPanel;
    ConsolePanel m_consolePanel;
    StartPanel m_startPanel;
    WorldHierarchyPanel m_hierarchyPanel;
    ContentBrowser m_contentBrowser;
    EditorCamera m_editorCamera;
    CameraController m_cameraController;
    std::vector<EditorPopup *> m_popups;

    World m_world;
    SceneState m_sceneState = SceneState::EDIT;
};

} // namespace Panda