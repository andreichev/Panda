//
// Created by Michael Andreichev on 14.09.2023.
//

#pragma once

#include "UI/Gizmos/Gizmos.hpp"
#include "Camera/CameraController.hpp"

#include <Panda/Base/Base.hpp>
#include <Panda/GameLogic/Camera.hpp>
#include <Miren/Miren.hpp>
#include <unordered_set>

namespace Panda {

class ViewportOutput {
public:
    virtual ~ViewportOutput() = default;
    virtual std::unordered_set<UUID> viewportGetSelectedIds() = 0;
    virtual void viewportPickEntitiesWithId(std::unordered_set<UUID> ids) = 0;
    virtual void viewportUnselectEntitiesWithId(std::unordered_set<UUID> ids) = 0;
    virtual void viewportUnselectAll() = 0;
};

class Viewport final {
    struct RectSelection {
        bool isStarted = false;
        bool appendSelection = false;
        Rect rect;
        std::unordered_set<UUID> initialSelection;
        std::unordered_set<UUID> currentSelection;
    };

public:
    Viewport(ViewportOutput *output, CameraController *cameraController);

    void initWithSize(Vec2 size);
    void setCamera(Camera *camera);
    void setWorld(World *world);
    void onImGuiRender(SceneState sceneState, float offsetY, bool fullScreen);
    void focus();
    bool isFocused();
    bool isHovered();
    Miren::ViewId getMirenView();

private:
    void updateViewportSize(Size size);
    void beginRectSelection(bool appendSelection);
    void updateRectSelection();
    void endRectSelection();
    std::unordered_set<UUID> getEntitiesInsideRectSelection();

    ViewportOutput *m_output;
    Gizmos m_gizmos;
    Miren::FrameBufferHandle m_sceneFB;
    Miren::FrameBufferSpecification m_sceneFbSpecification;
    Miren::ViewId m_sceneView;
    Miren::TextureHandle m_colorAttachment;
    Camera *m_camera;
    CameraController *m_cameraController;
    bool m_focusNextFrame;
    bool m_focused;
    bool m_hovered;
    Foundation::Memory m_idsBuffer;
    Rect m_frame;
    RectSelection m_rectSelection;
};

} // namespace Panda
