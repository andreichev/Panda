#include "Viewport.hpp"

#include <Panda.hpp>
#include <PandaUI/PandaUI.hpp>
#include <imgui.h>
#include <Panda/ImGui/FontAwesome.h>

namespace Panda {

Viewport::Viewport(ViewportOutput *output, CameraController *cameraController)
    : m_output(output)
    , m_frame()
    , m_hoveredId(-1)
    , m_camera(nullptr)
    , m_cameraController(cameraController)
    , m_focused(false)
    , m_focusNextFrame(true)
    , m_sceneFB()
    , m_sceneFbSpecification()
    , m_sceneView(1)
    , m_colorAttachment()
    , m_gizmos(nullptr, cameraController) {}

void Viewport::initWithSize(Vec2 size) {
    m_frame.size = size;
    PandaUI::initialize();
    Vec2 dpi = Application::get()->getWindow()->getDpi();
    Vec2 windowSize = Application::get()->getWindow()->getSize();
    Miren::TextureCreate create;
    create.m_data = Foundation::Memory(nullptr);
    create.m_format = Miren::TextureFormat::RGBA8;
    create.m_width = m_frame.size.width * dpi.width;
    create.m_height = m_frame.size.height * dpi.height;
    m_colorAttachment = Miren::createTexture(create);
    create.m_format = Miren::TextureFormat::RED_INTEGER;
    Miren::TextureHandle idAttachment = Miren::createTexture(create);
    create.m_format = Miren::TextureFormat::DEPTH24STENCIL8;
    Miren::TextureHandle depthAttachment = Miren::createTexture(create);
    Miren::FrameBufferAttachment attachments[] = {m_colorAttachment, idAttachment, depthAttachment};
    m_sceneFbSpecification = Miren::FrameBufferSpecification(attachments, 3);
    m_sceneFB = Miren::createFrameBuffer(m_sceneFbSpecification);
    Miren::setViewport(
        m_sceneView,
        Miren::Rect(0, 0, m_frame.size.width * dpi.width, m_frame.size.height * dpi.height)
    );
    Miren::setViewport(
        0, Miren::Rect(0, 0, windowSize.width * dpi.width, windowSize.height * dpi.height)
    );
    Miren::setViewFrameBuffer(m_sceneView, m_sceneFB);
    Miren::setViewClear(m_sceneView, 0x12212bff);
    Miren::setViewClearAttachments(m_sceneView, {Miren::Clear(1, -1)});
    PandaUI::Context::shared().updateViewId(m_sceneView);
}

void Viewport::updateViewportSize(Size size) {
    if (size.width < 1 || size.height < 1) {
        return;
    }
    m_frame.size = size;
    PandaUI::Context::shared().updateViewportSize({size.width, size.height});
    if (m_camera) {
        m_camera->setViewportSize(size);
    }
    Size dpi = Application::get()->getWindow()->getDpi();
    Miren::setViewport(
        m_sceneView, Miren::Rect(0, 0, size.width * dpi.width, size.height * dpi.height)
    );
    // COLOR ATTACHMENT
    Miren::resizeTexture(
        m_sceneFbSpecification.attachments[0].handle,
        size.width * dpi.width,
        size.height * dpi.height
    );
    // ID ATTACHMENT
    Miren::resizeTexture(
        m_sceneFbSpecification.attachments[1].handle,
        size.width * dpi.width,
        size.height * dpi.height
    );
    // DEPTH ATTACHMENT
    Miren::resizeTexture(
        m_sceneFbSpecification.attachments[2].handle,
        size.width * dpi.width,
        size.height * dpi.height
    );
    Miren::deleteFrameBuffer(m_sceneFB);
    m_sceneFB = Miren::createFrameBuffer(m_sceneFbSpecification);
    Miren::setViewFrameBuffer(m_sceneView, m_sceneFB);
}

void Viewport::onImGuiRender(SceneState sceneState, float offsetY, bool fullScreen) {
    ImGuiWindowFlags window_flags = 0;
    if (fullScreen) {
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImVec2 pos = viewport->WorkPos;
        pos.y += offsetY;
        ImGui::SetNextWindowPos(pos);
        ImVec2 size = viewport->WorkSize;
        size.y -= offsetY;
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                       ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
    ImGui::Begin(fullScreen ? "Viewport (fullscreen mode)" : "Viewport", NULL, window_flags);
    ImVec2 viewportSpace = ImGui::GetContentRegionAvail();
    viewportSpace = ImVec2(viewportSpace.x - 4, viewportSpace.y - 4);
    if (m_frame.size != viewportSpace) {
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
    m_frame.origin = ImGui::GetCursorScreenPos();
    Input::setViewportFrame(m_frame);
    ImGui::Image((void *)(uintptr_t)m_colorAttachment.id, m_frame.size, ImVec2(0, 1), ImVec2(1, 0));
    m_gizmos.onImGuiRender(sceneState, m_frame);

    ImGuiStyle &style = ImGui::GetStyle();
    float lastFrameRounding = style.FrameRounding;
    style.FrameRounding = 3.0f;
    ImGui::SetCursorPos({10, 40});
    if (ImGui::Button(getString(ICON_HOME).c_str())) {
        m_cameraController->reset({0, 0, 0});
    }
    style.FrameRounding = lastFrameRounding;


    ImGui::End();
    ImGui::PopStyleVar();

    Size dpi = Application::get()->getWindow()->getDpi();
    Miren::readFrameBuffer(
        m_sceneFB,
        1,
        Input::getMouseViewportPositionX() * dpi.x,
        m_frame.size.height * dpi.y - Input::getMouseViewportPositionY() * dpi.y,
        1,
        1,
        &m_hoveredId
    );

    if (m_focused && Input::isMouseButtonJustPressed(MouseButton::LEFT) && !m_gizmos.isUsing()) {
        if (m_hoveredId == -1) {
            m_output->viewportUnselectEntity();
        } else {
            m_output->viewportPickEntityWithEnttId(m_hoveredId);
        }
    }
    // LOG_EDITOR("id: {}", m_hoveredId);
}

void Viewport::setCamera(Camera *camera) {
    m_camera = camera;
    m_gizmos.setCamera(camera);
    if (m_camera) {
        m_camera->setViewportSize(m_frame.size);
    }
}

void Viewport::setWorld(World *world) {
    m_gizmos.setWorld(world);
}

bool Viewport::isFocused() {
    return m_focused;
}

int32_t Viewport::getHoveredId() {
    return m_hoveredId;
}

void Viewport::focus() {
    m_focusNextFrame = true;
}

Miren::ViewId Viewport::getMirenView() {
    return m_sceneView;
}

} // namespace Panda