#pragma once

#include "EditorPopup.hpp"
#include "ProjectLoader/AssetHandlerEditor.hpp"

#include <string>
#include <imgui.h>

namespace Panda {

void updateStringSize(std::string &str) {
    size_t actualLength = std::strlen(str.c_str());
    if (actualLength < str.size()) { str.resize(actualLength); }
}

class LocateAssetPopup : public EditorPopup {
public:
    void onImGuiRender() override {
        ImGui::OpenPopup(title);
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);
        if (ImGui::BeginPopupModal(title, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::TextUnformatted(subtitle);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

            for (int i = 0; i < missingFiles.size(); i++) {
                std::string &path = missingFiles[i];
                path.resize(100);
                char label[50];
                snprintf(label, 45, "path %d", i + 1);
                ImGui::InputText(label, path.data(), path.size());
                ImGui::SameLine();
                snprintf(label, 45, "Locate %d", i + 1);
                if (ImGui::Button(label)) {
                    auto pickedFile = SystemTools::openFileDialog();
                    if (pickedFile) {
                        path_t value = pickedFile.value();
                        path_t assetPath = std::filesystem::relative(value, projectPath);
                        if (!assetPath.empty()) { path = assetPath.string(); }
                    }
                }
            }

            if (ImGui::Button("Done", {ImGui::GetContentRegionAvail().x, 24})) {
                if (doneAction) {
                    for (int i = 0; i < missingFiles.size(); i++) {
                        std::string &path = missingFiles[i];
                        if (path.empty()) { break; }
                        updateStringSize(path);
                    }
                    doneAction();
                }
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

    const char *subtitle;
    MissingFiles missingFiles;
    path_t projectPath;
    PopupActionFunction doneAction;
    PopupActionFunction closeAction;
};

} // namespace Panda