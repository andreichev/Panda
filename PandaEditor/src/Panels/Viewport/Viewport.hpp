//
// Created by Michael Andreichev on 14.09.2023.
//

#pragma once

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
    Viewport(ViewportOutput *output);

    void initWithSize(Vec2 size);
    void setCamera(Camera *camera);
    void onImGuiRender();
    void focus();
    bool isFocused();
    id_t getHoveredId();
    Miren::ViewId getMirenView();

private:
    void updateViewportSize(Size size);

    ViewportOutput *m_output;
    Miren::FrameBufferHandle m_sceneFB;
    Miren::FrameBufferSpecification m_sceneFbSpecification;
    Miren::ViewId m_sceneView;
    Miren::TextureHandle m_colorAttachment;
    Camera *m_camera;
    Size m_viewportPanelSize;
    bool m_focusNextFrame;
    bool m_focused;
    id_t m_hoveredId;
};

} // namespace Panda
