//
// Created by Michael Andreichev on 14.09.2023.
//

#pragma once

#include "UI/Gizmos/Gizmos.hpp"
#include "Camera/CameraController.hpp"

#include <Viewport/Viewport.hpp>
#include <Panda/GameLogic/Camera.hpp>
#include <Miren/Miren.hpp>

namespace Panda {

class ViewportPanelOutput {
public:
    virtual ~ViewportPanelOutput() = default;
    virtual std::unordered_set<UUID> viewportGetSelectedIds() = 0;
    virtual void viewportPickEntitiesWithId(std::unordered_set<UUID> ids) = 0;
    virtual void viewportUnselectEntitiesWithId(std::unordered_set<UUID> ids) = 0;
    virtual void viewportUnselectAll() = 0;
};

class ViewportPanel final {
    struct RectSelection {
        bool isStarted = false;
        bool appendSelection = false;
        Rect rect;
        std::unordered_set<UUID> initialSelection;
        std::unordered_set<UUID> currentSelection;
    };

public:
    ViewportPanel(ViewportPanelOutput *output, CameraController *cameraController);
    ~ViewportPanel();

    void initWithSize(Vec2 size);
    void setCamera(Camera *camera);
    void setWorld(World *world);
    void onImGuiRender(SceneState sceneState, float offsetY, bool fullScreen);
    void viewportDrawOutline();
    void focus();
    bool isFocused();
    bool isHovered();
    Miren::ViewId getMirenView();

private:
    void updateViewportSize(Size size);
    void beginRectSelection(bool appendSelection);
    void updateRectSelection();
    void endRectSelection();

    ViewportPanelOutput *m_output;
    Viewport m_viewport;
    Gizmos m_gizmos;
    Camera *m_camera;
    CameraController *m_cameraController;
    bool m_focusNextFrame;
    bool m_focused;
    bool m_hovered;
    RectSelection m_rectSelection;
};

} // namespace Panda
