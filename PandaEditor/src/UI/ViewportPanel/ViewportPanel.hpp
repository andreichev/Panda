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

class ViewportPanel final {
    struct RectSelection {
        bool isStarted = false;
        bool appendSelection = false;
        Rect rect;
        std::unordered_set<UUID> initialSelection;
        std::unordered_set<UUID> currentSelection;
    };

public:
    ViewportPanel(CameraController *cameraController);
    ~ViewportPanel();

    void initWithSize(Vec2 size);
    void setCamera(Camera *camera);
    void setWorld(World *world);
    void onImGuiRender(SceneState sceneState, float offsetY, bool fullScreen);
    void drawOutline();
    void focus();
    bool isFocused();
    bool isHovered();
    Miren::ViewId getMirenView();

private:
    void updateViewportSize(Size size);
    void beginRectSelection(bool appendSelection);
    void updateRectSelection();
    void endRectSelection();
    std::unordered_set<UUID> getSelectedIds();
    void pickEntitiesWithId(std::unordered_set<UUID> ids);
    void unselectEntitiesWithId(std::unordered_set<UUID> ids);
    void unselectAll();

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
