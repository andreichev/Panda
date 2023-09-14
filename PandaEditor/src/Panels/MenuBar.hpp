//
// Created by Michael Andreichev on 14.09.2023.
//

#pragma once

#include <imgui.h>

namespace Panda {

class MenuBar final {
public:
    static void onImGuiRender() {
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
    }
};

} // namespace Panda