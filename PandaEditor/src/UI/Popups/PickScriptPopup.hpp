#pragma once

#include "EditorPopup.hpp"

#include <Panda/GameLogic/Entity.hpp>
#include <imgui.h>

namespace Panda {

using SelectScriptFunction =
    std::function<void(const std::vector<Entity> &entities, ScriptClassManifest clazz)>;

class PickScriptPopup final : public EditorPopup {
public:
    void onImGuiRender() override {
        static int selectedClassIndex = -1;
        static ScriptClassManifest selectedClass;
        if (!GameContext::s_scriptEngine || !GameContext::s_scriptEngine->isLoaded()) {
            PND_ASSERT(false, "SCRIPT ENGINE IS NOT INITIALIZED");
            return;
        }
        ImGui::OpenPopup(title);
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);
        if (ImGui::BeginPopupModal(title, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("%s", subtitle);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
            auto manifest = GameContext::s_scriptEngine->getManifest();
            // SCRIPT LIST
            for (int i = 0; i < manifest.classes.size(); i++) {
                if (ImGui::Selectable(manifest.classes[i].name, selectedClassIndex == i)) {
                    selectedClassIndex = i;
                    selectedClass = manifest.classes[i];
                }
            }
            if (ImGui::Button("Select", {ImGui::GetContentRegionAvail().x, 24})) {
                if (selectAction) { selectAction(entities, selectedClass); }
                ImGui::CloseCurrentPopup();
                ImGui::PopStyleVar(2);
                ImGui::EndPopup();
                selectedClassIndex = -1;
                selectedClass.name = nullptr;
                selectedClass.fields = {};
                return;
            }
            ImGui::Separator();
            if (ImGui::Button("Cancel", {ImGui::GetContentRegionAvail().x, 24})) {
                if (closeAction) { closeAction(); }
                ImGui::CloseCurrentPopup();
                ImGui::PopStyleVar(2);
                ImGui::EndPopup();
                selectedClassIndex = -1;
                selectedClass.name = nullptr;
                selectedClass.fields = {};
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
    std::vector<Entity> entities;
};

} // namespace Panda