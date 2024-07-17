#include "MenuBar.hpp"

namespace Panda {

MenuBar::MenuBar(Panda::MenuBarOutput *output)
    : m_output(output)
    , m_height(24) {}

void MenuBar::onImGuiRender() {
    ImGui::SetNextWindowPos({0, 0});
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImVec2 size = viewport->WorkSize;
    ImGui::SetNextWindowSize({size.x, m_height});
    ImGui::SetNextWindowViewport(viewport->ID);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin(
        "Menu bar",
        0,
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking
    );
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save World", NULL)) {
                m_output->menuBarSaveWorld();
            }
            if (ImGui::MenuItem("Open Project", NULL)) {
                m_output->menuBarOpenProject();
            }
            if (ImGui::MenuItem("Close Project", NULL)) {
                m_output->menuBarCloseProject();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", NULL)) {
                m_output->menuBarCloseApp();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::PopStyleVar(2);
    ImGui::End();
}

} // namespace Panda