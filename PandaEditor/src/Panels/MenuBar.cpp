#include "MenuBar.hpp"

#include <Foundation/PlatformDetection.hpp>

namespace Panda {

MenuBar::MenuBar(MenuBarOutput *output)
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
    ImGuiStyle &style = ImGui::GetStyle();
    float lastFrameRounding = style.FrameRounding;
    style.PopupRounding = 5.0;
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save World", NULL)) {
                m_output->menuBarSaveWorld();
            }
            if (ImGui::MenuItem("Open Project", NULL)) {
                m_output->menuBarOpenProject();
            }
            const std::vector<RecentProject> &recentProjects =
                m_output->menuBarGetRecentProjectsList();
            if (!recentProjects.empty()) {
                if (ImGui::BeginMenu("Recent Projects")) {
                    for (auto &project : recentProjects) {
                        if (project.path.empty()) {
                            continue;
                        }
                        path_t projectPath = project.path;
                        std::string projectName = projectPath.filename().string();
                        path_t openedProjectPath = m_output->menuBarGetOpenedProjectPath();
                        if (ImGui::MenuItem(
                                projectName.c_str(), nullptr, projectPath == openedProjectPath
                            )) {
                            if (projectPath != openedProjectPath) {
                                m_output->menuBarOpenProject(project);
                            }
                        }
                    }
                    ImGui::EndMenu();
                }
            }
#ifdef PLATFORM_MACOS
            if (ImGui::MenuItem("Open C++ Project", NULL)) {
                m_output->menuBarOpenCppProject();
            }
#endif
            if (ImGui::MenuItem("Close Project", NULL)) {
                m_output->menuBarCloseProject();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", NULL)) {
                m_output->menuBarCloseApp();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", NULL, false, m_output->menuBarCanUndo())) {
                m_output->menuBarUndo();
            }
            if (ImGui::MenuItem("Redo", NULL, false, m_output->menuBarCanRedo())) {
                m_output->menuBarRedo();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::PopStyleVar(2);
    style.PopupRounding = lastFrameRounding;
    ImGui::End();
}

} // namespace Panda