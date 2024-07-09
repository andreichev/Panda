//
// Created by Michael Andreichev on 14.09.2023.
//

#pragma once

#include <imgui.h>

namespace Panda {

class Dockspace final {
public:
    void beginImGuiDockspace(float offsetY) {
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImVec2 pos = viewport->WorkPos;
        pos.y += offsetY;
        ImGui::SetNextWindowPos(pos);
        ImVec2 size = viewport->WorkSize;
        size.y -= offsetY;
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        ImGui::Begin("DockSpace Frame", NULL, window_flags);
        ImGui::PopStyleVar(3);
        ImGuiID dockspace_id = ImGui::GetID("DockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));
    }

    void endImGuiDockspace() {
        ImGui::End();
    }
};

} // namespace Panda
