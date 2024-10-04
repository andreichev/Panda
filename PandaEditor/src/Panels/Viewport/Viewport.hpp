//
// Created by Michael Andreichev on 14.09.2023.
//

#pragma once

#include <Panda.hpp>

namespace Panda {

class Viewport final {
public:
    Viewport();

    void initWithSize(Vec2 size);
    void setCamera(Camera *camera);
    void onImGuiRender();
    void focus();
    bool isFocused();
    Miren::ViewId getMirenView();

private:
    void updateViewportSize(Size size);

    Miren::FrameBufferHandle m_sceneFB;
    Miren::FrameBufferSpecification m_sceneFbSpecification;
    Miren::ViewId m_sceneView;
    Miren::TextureHandle m_colorAttachment;
    Camera *m_camera;
    Size m_viewportPanelSize;
    bool m_focusNextFrame;
    bool m_focused;
};

} // namespace Panda
