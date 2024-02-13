//
// Created by Michael Andreichev on 14.09.2023.
//

#pragma once

#include <Panda.hpp>

namespace Panda {

class Viewport final {
public:
    Viewport()
        : m_viewportPanelSize(300, 200)
        , m_camera(nullptr) {}

    void init(World *world);
    void setCamera(Camera *camera);
    void onImGuiRender();

private:
    void updateViewportSize(Size size);

    World *m_world;
    Miren::FrameBufferHandle m_sceneFB;
    Miren::FrameBufferSpecification m_sceneFbSpecification;
    Miren::ViewId m_sceneViewId = 1;
    Miren::TextureHandle m_colorAttachment;
    Camera *m_camera;
    Size m_viewportPanelSize;
};

} // namespace Panda
