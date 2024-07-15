#pragma once

#include <imgui.h>

#include "Model/RecentProject.hpp"
#include "Panda/ImGui/FontAwesome.h"
#include "Panda/ImGui/ImGuiFonts.hpp"
#include "Panda/Application/Application.hpp"

namespace Panda {

class StartPanel final {
public:
    StartPanel()
        : m_logoImage("ui/start/Logo.png")
        , m_iconProject("ui/start/Icon Project.png")
        , m_recentProjects() {}

    void onImGuiRender() {
        ImGui::ShowDemoWindow();
        ImGui::SetNextWindowPos({0, 0});
        auto size = Application::get()->getWindow()->getSize();
        auto wflags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration;
        ImGui::SetNextWindowSize({size.width, size.height});
        ImGui::Begin("Start panel", nullptr, wflags);
        // Left
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 25));
            ImGui::BeginChild("left pane", ImVec2(177, 0), true);
            ImGui::SetCursorPosX((177 - 50) / 2);
            ImGui::Image((void *)(uintptr_t)m_logoImage.getHandle().id, {50, 50});
            const char *title = "Panda v 0.1";
            auto textWidth = ImGui::CalcTextSize(title).x;
            ImGui::SetCursorPosX((177 - textWidth) * 0.5f);
            ImGui::Text(title, nullptr);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 28);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
            ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0, 0.5});
            ImGui::Button((getString(ICON_PLUS) + "  New Project").c_str(), {ImGui::GetContentRegionAvail().x, 40});
            ImGui::Button((getString(ICON_FOLDER_OPEN) + "  Open Project").c_str(), {ImGui::GetContentRegionAvail().x, 40});
            ImGui::PopStyleVar(2);
            ImGui::EndChild();
            ImGui::PopStyleVar();
        }
        ImGui::SameLine();
        // Right
        {
            ImGui::BeginChild("right pane", ImVec2(0, -ImGui::GetFrameHeight()));
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8);
            ImGui::Text("Recent Projects");
            ImGui::Separator();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);
            if (m_recentProjects.empty()) {
                Fonts::pushFont("Bold");
                ImGui::TextColored({0.5, 0.5, 0.5, 1.0}, "No recent projects");
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 18);
                ImGui::Button("Create New Project", {ImGui::GetContentRegionAvail().x, 40});
                Fonts::popFont();
            }
            for (int i = 0; i < m_recentProjects.size(); i++) {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4);
                ImGui::Image((void *)(uintptr_t)m_iconProject.getHandle().id, {40, 40});
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Button, {0, 0, 0, 0});
                ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0, 0.5});
                if (ImGui::Button(
                        m_recentProjects[i].name.c_str(),
                        {ImGui::GetContentRegionAvail().x - 55, 40}
                    )) {
                    LOG_INFO("OPEN PROJECT {}", m_recentProjects[i].name);
                }
                ImGui::PopStyleVar();
                ImGui::SameLine();
                if (ImGui::Button(getString(ICON_WINDOW_CLOSE).c_str(), {40, 40})) {
                    LOG_INFO("REMOVE RECENT PROJECT {}", m_recentProjects[i].name);
                }
                ImGui::PopStyleColor();
            }
            ImGui::EndChild();
        }
        ImGui::End();
    }

    void setRecentProjects(std::vector<RecentProject> recentProjects) {
        m_recentProjects = recentProjects;
    }

private:
    Texture m_logoImage;
    Texture m_iconProject;
    std::vector<RecentProject> m_recentProjects;
};

} // namespace Panda