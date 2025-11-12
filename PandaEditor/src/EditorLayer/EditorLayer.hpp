#pragma once

#include "Camera/EditorCamera.hpp"
#include "Camera/CameraController.hpp"
#include "EditorLayer/SceneState.hpp"
#include "UI/Popups/EditorPopup.hpp"
#include "UI/PanelsContainer/PanelsContainer.hpp"
#include "UI/Other/StartPanel.hpp"
#include "ProjectLoader/ProjectLoader.hpp"
#include "SceneGrid.hpp"

#include <Panda/Application/Layer.hpp>

namespace Panda {

class EditorLayer : public Layer,
                    public ProjectLoaderOutput,
                    public MenuBarOutput,
                    public ComponentsDrawOutput,
                    public AssetPropertiesDrawOutput,
                    public ContentBrowserOutput,
                    public ToolbarOutput {
public:
    EditorLayer(Fern::Window *window);
    ~EditorLayer() override = default;

    virtual void onAttach() override;
    virtual void onDetach() override;

    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onEvent(Fern::Event *event) override;

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
    void menuBarShowCppProject() override;
    void menuBarSaveWorld() override;
    void menuBarCloseProject() override;
    void menuBarAbout() override;
#pragma endregion

#pragma region Components draw output
    void addScriptToEntities(const std::unordered_set<Entity> &entities) override;
#pragma endregion

#pragma region Asset properties draw output
    void locateMissingAsset(AssetId id) override;
#pragma endregion

#pragma region Content browser output
    void showCreateFolderPopup() override;
    void deleteFileShowPopup(path_t path) override;
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

private:
    void windowSizeChanged(Size size);
    void closeProject();
    void closeAppRequest();
    void closeApp();
    void saveWorld();
    bool canUndo();
    void undo();
    bool canRedo();
    void redo();
    void updateWindowState();
    void processShortcuts();
    void openProjectFolderDialog();

    bool m_viewportFullscreen;
    ProjectLoader m_loader;
    StartPanel m_startPanel;
    EditorCamera m_editorCamera;
    CameraController m_cameraController;
    std::list<EditorPopup *> m_popups;
    SceneGrid m_grid;
    Fern::Window *m_window;
    PanelsContainer m_panelsContainer;

    // World that is visible by default in editor but never played.
    World m_editingWorld;
    // World that is visible only when state is PLAY.
    World m_playingWorld;
    World *m_currentWorld;
    SceneState m_sceneState;
};

} // namespace Panda