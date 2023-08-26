#include "EditorLayer.hpp"
#include "Components/OrthographicCameraMove.hpp"

#include <imgui.h>

namespace Panda {

EditorLayer::EditorLayer()
    : m_viewportPanelSize(300, 200)
    , m_camera(nullptr)
    , m_world(nullptr) {}

void EditorLayer::initializeWorld() {
    if (m_camera == nullptr) {
        Entity cameraEntity = m_world->instantiateEntity();
        m_camera = &cameraEntity.addNativeScript<OrthographicCamera>();
        auto &move = cameraEntity.addNativeScript<OrthographicCameraMove>();
        move.setCamera(m_camera);
    }
}

void EditorLayer::onAttach() {
    m_world = NEW(Foundation::getAllocator(), World);
    initializeWorld();
    Size dpi = Application::get()->getWindow()->getDpi();
    Size windowSize = Application::get()->getWindow()->getSize();
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
    m_camera->updateViewportSize(m_viewportPanelSize);
    Renderer2D::setCamera(m_camera);
    Miren::setViewFrameBuffer(m_sceneViewId, m_sceneFB);
    Renderer2D::setViewId(m_sceneViewId);
}

void EditorLayer::onDetach() {
    DELETE(Foundation::getAllocator(), m_world);
}

void EditorLayer::onUpdate(double deltaTime) {
    m_world->update(deltaTime);

    Renderer2D::RectData rect2;
    rect2.color = Color(1.0f, 0.f, 0.f, 1.f);
    rect2.origin = Point(0.6f, 0.6f);
    rect2.size = Size(1.f, 1.f);
    Renderer2D::drawRect(rect2);

    Renderer2D::RectData rect3;
    rect3.color = Color(1.f, 0.f, 1.f, 1.f);
    rect3.origin = Point(0.0f, 0.0f);
    rect3.size = Size(1.0f, 1.0f);
    Renderer2D::drawRect(rect3);

    if (Input::isKeyJustPressed(Key::ESCAPE)) {
        Application::get()->close();
    }
}

void EditorLayer::onImGuiRender() {
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Frame", NULL, window_flags);
    ImGui::PopStyleVar(3);
    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Options")) {
            ImGui::MenuItem("Save", NULL);
            ImGui::MenuItem("Open", NULL);
            ImGui::Separator();

            if (ImGui::MenuItem("Exit", NULL)) {
                Application::get()->close();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    statisticsPanel.onImGuiRender();

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

    // DOCKSPACE
    ImGui::End();
}

void EditorLayer::updateViewportSize(Size size) {
    if (size.width == 0 || size.height == 0) {
        return;
    }
    m_viewportPanelSize = size;
    m_camera->updateViewportSize(size);
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

void EditorLayer::onEvent(Event *event) {
    Input::onEvent(event);
}

} // namespace Panda
