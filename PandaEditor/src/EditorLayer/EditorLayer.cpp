#include "EditorLayer.hpp"

#include <imgui.h>

namespace Panda {

EditorLayer::EditorLayer()
    : m_viewportPanelSize(300, 200) {}

void EditorLayer::onAttach() {
    Panda::Size dpi = Panda::Application::get()->getWindow()->getDpi();
    Panda::Size windowSize = Panda::Application::get()->getWindow()->getSize();
    Miren::TextureCreate create;
    create.m_data = Foundation::Memory(nullptr);
    create.m_format = Miren::TextureFormat::RGBA8;
    create.m_width = m_viewportPanelSize.width * dpi.width;
    create.m_height = m_viewportPanelSize.height * dpi.height;
    m_colorAttachment = Miren::createTexture(create);
    create.m_format = Miren::TextureFormat::DEPTH24STENCIL8;
    Miren::TextureHandle depthAttachment = Miren::createTexture(create);
    Miren::FrameBufferAttachment attachments[] = {m_colorAttachment, depthAttachment};
    Miren::FrameBufferSpecification spec(attachments, 2);
    m_frameBuffer = Miren::createFrameBuffer(spec);
    Miren::setViewport(m_sceneViewId,
        Miren::Rect(
            0, 0, m_viewportPanelSize.width * dpi.width, m_viewportPanelSize.height * dpi.height));
    Miren::setViewport(
        0, Miren::Rect(0, 0, windowSize.width * dpi.width, windowSize.height * dpi.height));
    Miren::setViewClear(m_sceneViewId, 0x111111ff);
}

void EditorLayer::onDetach() {}

void EditorLayer::onUpdate(double deltaTime) {
    Panda::Renderer2D::setFrameBuffer(m_frameBuffer);
    Panda::Renderer2D::setViewId(m_sceneViewId);

    if (Panda::Input::isKeyJustPressed(Panda::Key::ESCAPE)) {
        Panda::Application::get()->close();
    }
}

void EditorLayer::onImGuiRender() {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    bool d = true;
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
                Panda::Application::get()->close();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Begin("Renderer2D statistics");
    ImGui::Text("FPS: %d", Panda::Application::get()->fps);
    auto stats = Panda::Renderer2D::getStats();
    ImGui::Text("Quads count: %d", stats.quadCount);
    ImGui::Text("Vertices count: %d", stats.getTotalVertexCount());
    ImGui::Text("Indices count: %d", stats.getTotalIndexCount());
    ImGui::Text("Draw calls: %d", stats.drawCalls);
    ImGui::End();

    ImGui::Begin("Viewport");
    ImVec2 space = ImGui::GetContentRegionAvail();
    m_viewportPanelSize = Panda::Size(space.x, space.y);
    ImGui::Image((void *)(uintptr_t)m_colorAttachment,
        ImVec2(m_viewportPanelSize.width, m_viewportPanelSize.height),
        ImVec2(0, 1),
        ImVec2(1, 0));
    ImGui::End();

    // DOCKSPACE
    ImGui::End();
}

void EditorLayer::onEvent(Event *event) {
    Input::onEvent(event);
}

} // namespace Panda