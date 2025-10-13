//
// Created by Andreichev Mikhail on 23.03.2025.
//

#include "UI/Other/MenuBar.hpp"
#include "UI/Other/Dockspace.hpp"
#include "UI/Other/WorldHierarchyPanel.hpp"
#include "UI/Other/StatisticsPanel.hpp"
#include "UI/ViewportPanel/ViewportPanel.hpp"
#include "UI/Console/ConsolePanel.hpp"
#include "UI/Toolbar/Toolbar.hpp"
#include "UI/ContentBrowser/ContentBrowser.hpp"
#include "UI/Properties/PropertiesPanel.hpp"
#include "UI/AssetManager/AssetManagerPanel.hpp"

namespace Panda {

class EditorLayer;
class CameraController;
class ProjectLoader;

class PanelsContainer final {
public:
    PanelsContainer(
        EditorLayer *layer, ProjectLoader *projectLoader, CameraController *cameraController
    );
    void onImGuiRender(bool isViewportFullScreen, SceneState sceneState);
    void setCurrentWorld(World *world);

    MenuBar menuBar;
    Toolbar toolbar;
    Dockspace dockspace;
    ViewportPanel viewport;
    StatisticsPanel statisticsPanel;
    ConsolePanel consolePanel;
    WorldHierarchyPanel hierarchyPanel;
    ContentBrowser contentBrowser;
    PropertiesPanel propertiesPanel;
    AssetManagerPanel assetManagerPanel;
};

} // namespace Panda
