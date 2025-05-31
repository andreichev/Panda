//
// Created by Bogdan on 16.05.2025.
//

#pragma once

#include "EditorPopup.hpp"

#include <Panda/GameLogic/Input.hpp>
#include <Panda/Assets/TextureAsset.hpp>
#include "Panda/ImGui/ImGuiFonts.hpp"
#include <imgui.h>

namespace Panda {

class EditorAboutPopup : public EditorPopup {
public:
    void onImGuiRender() override {
        if (open) { ImGui::OpenPopup(title); }
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);
        if (ImGui::BeginPopupModal(title, &open, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
            Panda::Fonts::pushFont("BigBold");
            ImGui::Text("Panda");
            Panda::Fonts::popFont();

            ImGui::Image((ImTextureID)(intptr_t)pandaIcon.getMirenHandle().id, {90, 90});
            ImGui::Text("Open source 2D and 3D cross-platform game Environment. Developing by "
                        "company Toyship.");
            ImGui::Text(" ");
            ImGui::Text("Developers: Mikhail Andreichev, Bogdan Mikhailov");

            ImGui::TextLinkOpenURL("Git ", "https://github.com/andreichev/Panda");
            ImGui::SameLine();
            ImGui::TextLinkOpenURL(
                "License", "https://github.com/andreichev/Panda/blob/main/LICENSE.txt"
            );

            if (!open) {
                if (closeAction) { closeAction(); }
                ImGui::CloseCurrentPopup();
                ImGui::PopStyleVar(2);
                ImGui::EndPopup();
                return;
            }
            ImGui::PopStyleVar();
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }
        ImGui::PopStyleVar();
    }
    PopupActionFunction closeAction;

private:
    bool open = true;
    Panda::TextureAsset pandaIcon = Panda::TextureAsset("ui/icons/Panda.png");
};

} // namespace Panda
