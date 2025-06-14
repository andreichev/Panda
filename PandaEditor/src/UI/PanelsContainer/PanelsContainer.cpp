#include "EditorLayer/EditorLayer.hpp"

namespace Panda {

PanelsContainer::PanelsContainer(EditorLayer *layer, CameraController *cameraController)
    : toolbar(layer)
    , dockspace()
    , viewport(cameraController)
    , hierarchyPanel(nullptr, layer)
    , statisticsPanel(nullptr)
    , consolePanel()
    , menuBar(layer)
    , contentBrowser(layer) {}

void PanelsContainer::onImGuiRender(bool isViewportFullScreen, SceneState sceneState) {
    menuBar.onImGuiRender();
    toolbar.onImGuiRender(menuBar.m_height);
    float offsetY = toolbar.getHeight() + menuBar.m_height + 2;
    if (isViewportFullScreen) {
        viewport.onImGuiRender(sceneState, offsetY, true);
    } else {
        dockspace.beginImGuiDockspace(offsetY);
        statisticsPanel.onImGuiRender();
        viewport.onImGuiRender(sceneState, 0, false);
        hierarchyPanel.onImGuiRender();
        contentBrowser.onImGuiRender();
        consolePanel.onImGuiRender();
        dockspace.endImGuiDockspace();
    }
}

void PanelsContainer::setCurrentWorld(World *world) {
    hierarchyPanel.setWorld(world);
    statisticsPanel.setWorld(world);
    viewport.setWorld(world);
}

} // namespace Panda