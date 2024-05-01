#pragma once

#include "Panels/Viewport.hpp"
#include "Panels/WorldHierarchyPanel.hpp"
#include "Panels/StatisticsPanel.hpp"
#include "Camera/EditorCamera.hpp"
#include "Camera/CameraController.hpp"

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

    Viewport m_viewport;
    StatisticsPanel m_statisticsPanel;
    WorldHierarchyPanel m_hierarchyPanel;
    EditorCamera m_editorCamera;
    CameraController m_cameraController;
    World *m_world;

    enum class SceneState { EDIT = 0, PLAY = 1, SIMULATE = 2 };
    SceneState m_sceneState = SceneState::EDIT;
};

} // namespace Panda