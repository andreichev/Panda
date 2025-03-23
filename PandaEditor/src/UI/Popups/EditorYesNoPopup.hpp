#pragma once

#include "EditorPopup.hpp"

#include <Panda/GameLogic/Input.hpp>
#include <imgui.h>

namespace Panda {

class EditorYesNoPopup : public EditorPopup {
public:
    void onImGuiRender() override {
        ImGui::OpenPopup(title);
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);
        if (ImGui::BeginPopupModal(title, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("%s", subtitle.c_str());
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
            if (ImGui::Button(yesText.c_str(), {ImGui::GetContentRegionAvail().x, 24}) ||
                Input::isKeyPressed(Fern::Key::ENTER)) {
                if (yesAction) { yesAction(); }
                ImGui::CloseCurrentPopup();
                ImGui::PopStyleVar(2);
                ImGui::EndPopup();
                return;
            }
            if (ImGui::Button(noText.c_str(), {ImGui::GetContentRegionAvail().x, 24})) {
                if (noAction) { noAction(); }
                ImGui::CloseCurrentPopup();
                ImGui::PopStyleVar(2);
                ImGui::EndPopup();
                return;
            }
            ImGui::Separator();
            if (ImGui::Button("Cancel", {ImGui::GetContentRegionAvail().x, 24})) {
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

    std::string subtitle;
    std::string yesText;
    std::string noText;
    PopupActionFunction yesAction;
    PopupActionFunction noAction;
    PopupActionFunction closeAction;
};

} // namespace Panda