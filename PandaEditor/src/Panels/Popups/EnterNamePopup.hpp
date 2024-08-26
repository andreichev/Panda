#pragma once

#include "EditorPopup.hpp"

#include <string>
#include <imgui.h>

namespace Panda {

using EnteringDoneFunction = void (*)(void *userData, std::string text);

class EnterNamePopup : public EditorPopup {
public:
    void onImGuiRender() override {
        static std::string text;

        ImGui::OpenPopup(title);
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);
        if (ImGui::BeginPopupModal(title, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("%s", subtitle);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

            text.resize(40);
            ImGui::InputText("##", text.data(), text.size());

            if (ImGui::Button("Done", {ImGui::GetContentRegionAvail().x, 24})) {
                if (doneAction) {
                    std::string clearText = text.substr(0, text.find('\0'));
                    doneAction(userData, clearText);
                }
                ImGui::CloseCurrentPopup();
                ImGui::PopStyleVar(2);
                ImGui::EndPopup();
                text.clear();
                return;
            }
            ImGui::Separator();
            if (ImGui::Button("Cancel", {ImGui::GetContentRegionAvail().x, 24})) {
                if (closeAction) {
                    closeAction(userData);
                }
                ImGui::CloseCurrentPopup();
                ImGui::PopStyleVar(2);
                ImGui::EndPopup();
                text.clear();
                return;
            }

            ImGui::PopStyleVar();
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }
        ImGui::PopStyleVar();
    }

    const char *subtitle;
    EnteringDoneFunction doneAction;
    PopupActionFunction closeAction;
    void *userData;
};

} // namespace Panda