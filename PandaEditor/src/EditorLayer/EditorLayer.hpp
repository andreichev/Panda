#pragma once

#include "Panels/Viewport/Viewport.hpp"
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
#include "SceneGrid.hpp"

#include <Panda/Application/Layer.hpp>

namespace Panda {

class EditorLayer : public Layer,
                    public ProjectLoaderOutput,
                    public MenuBarOutput,
                    public ComponentsDrawOutput,
                    public ContentBrowserOutput,
                    public ToolbarOutput,
                    public ViewportOutput {
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
    bool menuBarCanUndo() override;
    void menuBarUndo() override;
    bool menuBarCanRedo() override;
    void menuBarRedo() override;
    const std::vector<RecentProject> &menuBarGetRecentProjectsList() override;
    const path_t &menuBarGetOpenedProjectPath() override;
    void menuBarCloseApp() override;
    void menuBarOpenCppProject() override;
    void menuBarSaveWorld() override;
    void menuBarCloseProject() override;
#pragma endregion

#pragma region Components draw output
    void addScriptToEntities(const std::vector<Entity> &entities) override;
#pragma endregion

#pragma region Content browser output
    void showCreateFolderPopup() override;
    void deleteFileShowPopup(path_t path) override;
    void importAsset(const path_t &path) override;
    UUID getAssetId(const path_t &path) override;
#pragma endregion

#pragma region Toolbar output
    bool toolbarCanUndo() override;
    void toolbarUndo() override;
    bool toolbarCanRedo() override;
    void toolbarRedo() override;
    void toolbarDidPickSceneState(SceneState state) override;
    void toolbarDidTapReloadScripts() override;
    SceneState toolbarGetCurrentSceneState() override;
#pragma endregion

#pragma region Viewport output
    void viewportPickEntityWithId(UUID id) override;
    void viewportUnselectAll() override;
#pragma endregion

private:
    void closeApp();
    void saveWorld();
    bool canUndo();
    void undo();
    bool canRedo();
    void redo();
    void updateWindowState();
    void processShortcuts();

    bool m_viewportFullscreen;
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
    SceneGrid m_grid;

    // World that is visible by default in editor but never played.
    World m_editingWorld;
    // World that is visible only when state is PLAY.
    World m_playingWorld;
    World *m_currentWorld;
    SceneState m_sceneState = SceneState::EDIT;
};

} // namespace Panda