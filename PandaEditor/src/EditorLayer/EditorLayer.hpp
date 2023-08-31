#pragma once

#include "Panels/StatisticsPanel.hpp"

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

    World *m_world;
    Miren::FrameBufferHandle m_sceneFB;
    Miren::FrameBufferSpecification m_sceneFbSpecification;
    Miren::ViewId m_sceneViewId = 1;
    Miren::TextureHandle m_colorAttachment;
    OrthographicCamera *m_camera;
    Size m_viewportPanelSize;

    // Panels
    StatisticsPanel statisticsPanel;
};

} // namespace Panda