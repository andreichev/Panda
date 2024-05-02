#pragma once

#include "Panels/Viewport.hpp"
#include "Panels/MenuBar.hpp"
#include "Panels/Dockspace.hpp"
#include "Panels/WorldHierarchyPanel.hpp"
#include "Panels/StatisticsPanel.hpp"
#include "Panels/Toolbar.hpp"
#include "Camera/EditorCamera.hpp"
#include "Camera/CameraController.hpp"
#include "EditorLayer/SceneState.hpp"

#include <Panda.hpp>

namespace Panda {

class EditorLayer : public Layer {
public:
    EditorLayer();
    virtual ~EditorLayer() = default;

    virtual void onAttach() override;
    virtual void onDetach() override;

    void onUpdate(double deltaTime) override;
    virtual void onImGuiRender() override;
    void onEvent(Event *event) override;

    void play();
    void stop();

private:
    void initializeEmptyWorld();

    MenuBar m_menuBar;
    Toolbar m_toolbar;
    Dockspace m_dockspace;
    Viewport m_viewport;
    StatisticsPanel m_statisticsPanel;
    WorldHierarchyPanel m_hierarchyPanel;
    EditorCamera m_editorCamera;
    CameraController m_cameraController;
    World *m_world;

    SceneState m_sceneState = SceneState::EDIT;
};

} // namespace Panda