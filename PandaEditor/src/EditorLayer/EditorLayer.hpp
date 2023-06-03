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
    Miren::FrameBufferHandle m_frameBuffer;
    Miren::ViewId m_sceneViewId = 1;
    Miren::TextureHandle m_colorAttachment;
    Panda::Size m_viewportPanelSize;
};

} // namespace Panda