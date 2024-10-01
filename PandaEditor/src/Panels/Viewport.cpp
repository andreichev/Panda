#include "Viewport.hpp"

#include <Panda.hpp>
#include <PandaUI/PandaUI.hpp>
#include <imgui.h>

namespace Panda {

Viewport::Viewport()
    : m_viewportPanelSize(300, 200)
    , m_camera(nullptr)
    , m_focused(false)
    , m_focusNextFrame(true)
    , m_sceneFB()
    , m_sceneFbSpecification()
    , m_sceneViewId(1)
    , m_colorAttachment() {}

void Viewport::init() {
    PandaUI::initialize();
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
    Miren::setViewport(
        m_sceneViewId,
        Miren::Rect(
            0, 0, m_viewportPanelSize.width * dpi.width, m_viewportPanelSize.height * dpi.height
        )
    );
    Miren::setViewport(
        0, Miren::Rect(0, 0, windowSize.width * dpi.width, windowSize.height * dpi.height)
    );
    Miren::setViewClear(m_sceneViewId, 0x12212bff);
    Miren::setViewFrameBuffer(m_sceneViewId, m_sceneFB);
    PandaUI::Context::shared().updateViewId(m_sceneViewId);
}

void Viewport::updateViewportSize(Vec2 size) {
    if (size.width < 1 || size.height < 1) {
        return;
    }
    m_viewportPanelSize = size;
    PandaUI::Context::shared().updateViewportSize({size.width, size.height});
    if (m_camera) {
        m_camera->setViewportSize(size);
    }
    Size dpi = Application::get()->getWindow()->getDpi();
    Miren::setViewport(
        m_sceneViewId, Miren::Rect(0, 0, size.width * dpi.width, size.height * dpi.height)
    );
    // COLOR ATTACHMENT
    Miren::resizeTexture(
        m_sceneFbSpecification.attachments[0].handle,
        size.width * dpi.width,
        size.height * dpi.height
    );
    // DEPTH ATTACHMENT
    Miren::resizeTexture(
        m_sceneFbSpecification.attachments[1].handle,
        size.width * dpi.width,
        size.height * dpi.height
    );
    Miren::deleteFrameBuffer(m_sceneFB);
    m_sceneFB = Miren::createFrameBuffer(m_sceneFbSpecification);
    Miren::setViewFrameBuffer(m_sceneViewId, m_sceneFB);
}

void Viewport::onImGuiRender() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
    ImGui::Begin("Viewport");
    ImVec2 viewportSpace = ImGui::GetContentRegionAvail();
    viewportSpace = ImVec2(viewportSpace.x - 4, viewportSpace.y - 4);
    if (m_viewportPanelSize != viewportSpace) {
        updateViewportSize(viewportSpace);
    }
    bool hovered = ImGui::IsWindowHovered();
    if (hovered && Input::isMouseButtonPressed(MouseButton::RIGHT) || m_focusNextFrame) {
        ImGui::SetWindowFocus();
        m_focusNextFrame = false;
    }
    m_focused = ImGui::IsWindowFocused();
    Application::get()->getImGuiLayer()->setBlockEvents(!hovered);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 1);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1);
    ImVec2 viewportScreenPos = ImGui::GetCursorScreenPos();
    Input::setViewportFrame(viewportScreenPos, viewportSpace);
    ImGui::Image(
        (void *)(uintptr_t)m_colorAttachment.id, m_viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0)
    );
    ImGui::End();
    ImGui::PopStyleVar();
}

void Viewport::setCamera(Camera *camera) {
    m_camera = camera;
    if (m_camera) {
        m_camera->setViewportSize(m_viewportPanelSize);
    }
}

bool Viewport::isFocused() {
    return m_focused;
}

void Viewport::focus() {
    m_focusNextFrame = true;
}

Miren::ViewId Viewport::getViewId() {
    return m_sceneViewId;
}

} // namespace Panda