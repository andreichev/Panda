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
    virtual void viewportPickEntityWithId(id_t id) = 0;
    virtual void viewportUnselectEntity() = 0;
};

class Viewport final {
public:
    Viewport(ViewportOutput *output, CameraController *cameraController);

    void initWithSize(Vec2 size);
    void setCamera(Camera *camera);
    void setWorld(World *world);
    void onImGuiRender(SceneState sceneState);
    void focus();
    bool isFocused();
    id_t getHoveredId();
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
    id_t m_hoveredId;
    Rect m_frame;
};

} // namespace Panda
