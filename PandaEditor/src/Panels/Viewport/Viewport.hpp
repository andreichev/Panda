//
// Created by Michael Andreichev on 14.09.2023.
//

#pragma once

#include "Panels/Gizmos/Gizmos.hpp"
#include "Camera/CameraController.hpp"

#include <Panda/Base/Base.hpp>
#include <Panda/GameLogic/Camera.hpp>
#include <Miren/Miren.hpp>

namespace Panda {

class ViewportOutput {
public:
    virtual ~ViewportOutput() = default;
    virtual void viewportPickEntityWithId(UUID id) = 0;
    virtual void viewportUnselectAll() = 0;
};

class Viewport final {
public:
    Viewport(ViewportOutput *output, CameraController *cameraController);

    void initWithSize(Vec2 size);
    void setCamera(Camera *camera);
    void setWorld(World *world);
    void onImGuiRender(SceneState sceneState, float offsetY, bool fullScreen);
    void focus();
    bool isFocused();
    int32_t getHoveredId();
    Miren::ViewId getMirenView();

private:
    void updateViewportSize(Size size);

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
    uint32_t m_hoveredId;
    Rect m_frame;
};

} // namespace Panda
