//
// Created by Michael Andreichev on 14.09.2023.
//

#pragma once

#include <Panda.hpp>

namespace Panda {

class Viewport final {
public:
    Viewport();

    void init();
    void setCamera(Camera *camera);
    void onImGuiRender();
    void focus();
    bool isFocused();
    Miren::ViewId getViewId();

private:
    void updateViewportSize(Size size);

    Miren::FrameBufferHandle m_sceneFB;
    Miren::FrameBufferSpecification m_sceneFbSpecification;
    Miren::ViewId m_sceneViewId;
    Miren::TextureHandle m_colorAttachment;
    Camera *m_camera;
    Size m_viewportPanelSize;
    bool m_focusNextFrame;
    bool m_focused;
};

} // namespace Panda
