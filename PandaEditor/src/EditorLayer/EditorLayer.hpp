#pragma once

#include <Panda.hpp>

namespace Panda {

class EditorLayer : public Layer {
public:
    EditorLayer();
    virtual ~EditorLayer() = default;

    virtual void onAttach() override;
    virtual void onDetach() override;

    void onUpdate(double deltaTime) override;
    virtual void onImGuiRender() override;
    void onEvent(Event *event) override;

private:
    void initializeWorld();
    void updateViewportSize(Size size);

    Panda::World *m_world;
    Miren::FrameBufferHandle m_sceneFB;
    Miren::FrameBufferSpecification m_sceneFbSpecification;
    Miren::ViewId m_sceneViewId = 1;
    Miren::TextureHandle m_colorAttachment;
    Panda::OrthographicCamera *m_camera;
    Panda::Size m_viewportPanelSize;
};

} // namespace Panda