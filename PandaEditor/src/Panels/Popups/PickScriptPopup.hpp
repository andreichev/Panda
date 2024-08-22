#pragma once

#include "EditorPopup.hpp"

#include <Panda/GameLogic/Entity.hpp>
#include <imgui.h>

namespace Panda {

using SelectScriptFunction = void (*)(void *userData, Entity entity, const char *scriptName);

class PickScriptPopup : public EditorPopup {
public:
    void onImGuiRender() override {
        static int selectedClassIndex = -1;
        static const char *selectedClassName = nullptr;
        ImGui::OpenPopup(title);
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);
        if (ImGui::BeginPopupModal(title, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("%s", subtitle);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

            auto classes = ExternalCalls::getAvailableScripts();
            // SCRIPT LIST
            for (int i = 0; i < classes.size(); i++) {
                if (ImGui::Selectable(classes[i], selectedClassIndex == i)) {
                    selectedClassIndex = i;
                    selectedClassName = classes[i];
                }
            }
            if (ImGui::Button("Select", {ImGui::GetContentRegionAvail().x, 24})) {
                if (selectAction) {
                    selectAction(userData, entity, selectedClassName);
                }
                ImGui::CloseCurrentPopup();
                ImGui::PopStyleVar(2);
                ImGui::EndPopup();
                selectedClassIndex = -1;
                selectedClassName = nullptr;
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
                selectedClassIndex = -1;
                selectedClassName = nullptr;
                return;
            }

            ImGui::PopStyleVar();
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }
        ImGui::PopStyleVar();
    }

    const char *subtitle;
    SelectScriptFunction selectAction;
    PopupActionFunction closeAction;
    void *userData;
    Entity entity;
};

} // namespace Panda