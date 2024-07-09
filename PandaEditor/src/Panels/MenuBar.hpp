//
// Created by Michael Andreichev on 14.09.2023.
//

#pragma once

#include <imgui.h>

namespace Panda {

class MenuBar final {
public:
    void onImGuiRender() {
        ImGui::SetNextWindowPos({0, 0});
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImVec2 size = viewport->WorkSize;
        ImGui::SetNextWindowSize({size.x, height});
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
            if (ImGui::BeginMenu("Options")) {
                if(ImGui::MenuItem("Save", NULL)) {

                }
                ImGui::MenuItem("Open", NULL);
                ImGui::Separator();

                if (ImGui::MenuItem("Exit", NULL)) {
                    Application::get()->close();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::PopStyleVar(2);
        ImGui::End();
    }

    float height = 24;
};

} // namespace Panda