#include "Viewport.hpp"

#include <Panda.hpp>
#include <PandaUI/PandaUI.hpp>
#include <imgui.h>

namespace Panda {

void Viewport::init(World *world) {
    PandaUI::initialize();
    m_world = world;
    Vec2 dpi = Application::get()->getWindow()->getDpi();
    Vec2 windowSize = Application::get()->getWindow()->getSize();
    Miren::TextureCreate create;
    create.m_data = Foundation::Memory(nullptr);
    create.m_format = Miren::TextureFormat::RGBA8;
    create.m_width = m_viewportPanelSize.width * dpi.width;
    create.m_height = m_viewportPanelSize.height * dpi.height;
    m_colorAttachment = Miren::createTexture(create);
    create.m_format = Miren::TextureFormat::DEPTH24STENCIL8;
    Miren::TextureHandle depthAttachment = Miren::createTexture(create);
    Miren::FrameBufferAttachment attachments[] = {m_colorAttachment, depthAttachment};
    m_sceneFbSpecification = Miren::FrameBufferSpecification(attachments, 2);
    m_sceneFB = Miren::createFrameBuffer(m_sceneFbSpecification);
    Miren::setViewport(m_sceneViewId,
        Miren::Rect(
            0, 0, m_viewportPanelSize.width * dpi.width, m_viewportPanelSize.height * dpi.height));
    Miren::setViewport(
        0, Miren::Rect(0, 0, windowSize.width * dpi.width, windowSize.height * dpi.height));
    Miren::setViewClear(m_sceneViewId, 0x12212bff);
    Miren::setViewFrameBuffer(m_sceneViewId, m_sceneFB);
    m_world->getRenderer2D().setViewId(m_sceneViewId);
    PandaUI::Context::shared().updateViewId(m_sceneViewId);
}

void Viewport::updateViewportSize(Vec2 size) {
    if (size.width < 1 || size.height < 1) {
        return;
    }
    m_viewportPanelSize = size;
    PandaUI::Context::shared().updateViewportSize({size.width, size.height});
    if (m_camera) {
        m_camera->viewportSizeChanged(size);
    }
    Size dpi = Application::get()->getWindow()->getDpi();
    Miren::setViewport(
        m_sceneViewId, Miren::Rect(0, 0, size.width * dpi.width, size.height * dpi.height));
    // COLOR ATTACHMENT
    Miren::resizeTexture(m_sceneFbSpecification.attachments[0].handle,
        size.width * dpi.width,
        size.height * dpi.height);
    // DEPTH ATTACHMENT
    Miren::resizeTexture(m_sceneFbSpecification.attachments[1].handle,
        size.width * dpi.width,
        size.height * dpi.height);
    Miren::deleteFrameBuffer(m_sceneFB);
    m_sceneFB = Miren::createFrameBuffer(m_sceneFbSpecification);
    Miren::setViewFrameBuffer(m_sceneViewId, m_sceneFB);
}

void Viewport::onImGuiRender() {
    ImGui::Begin("Viewport");
    ImVec2 viewportSpace = ImGui::GetContentRegionAvail();
    if (viewportSpace.x != m_viewportPanelSize.width ||
        viewportSpace.y != m_viewportPanelSize.height) {
        updateViewportSize({viewportSpace.x, viewportSpace.y});
    }
    bool viewportHovered = ImGui::IsWindowHovered();
    Application::get()->getImGuiLayer()->setBlockEvents(!viewportHovered);
    ImGui::Image((void *)(uintptr_t)m_colorAttachment.id,
        ImVec2(m_viewportPanelSize.width, m_viewportPanelSize.height),
        ImVec2(0, 1),
        ImVec2(1, 0));
    ImGui::End();
}

void Viewport::setCamera(CameraComponent *camera) {
    m_camera = camera;
    m_camera->viewportSizeChanged(m_viewportPanelSize);
    m_world->getRenderer2D().setCamera(m_camera);
}

} // namespace Panda