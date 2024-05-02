#pragma once

#include "Panels/Common/ImGuiHelper.hpp"
#include "EditorLayer/SceneState.hpp"

namespace Panda {

class Toolbar final {
public:
    SceneState onImGuiRender(float offsetY, SceneState lastSceneState) {
        SceneState pickedSceneState = lastSceneState;
        pushID();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowPos({0, offsetY});
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImVec2 size = viewport->WorkSize;
        float width = size.x;
        ImGui::SetNextWindowSize({width, height});
        ImGui::Begin(
            "##viewport_central_toolbar",
            0,
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking
        );
        ImGui::SetCursorPos({width / 2.f - 70.f, 4.f});
        switch (lastSceneState) {
            case SceneState::EDIT: {
                if (ImGui::Button("Play")) {
                    pickedSceneState = SceneState::PLAY;
                }
                ImGui::SameLine();
                if (ImGui::Button("Simulate")) {
                    pickedSceneState = SceneState::SIMULATE;
                }
                break;
            }
            case SceneState::SIMULATE:
            case SceneState::PLAY: {
                if (ImGui::Button("Stop")) {
                    pickedSceneState = SceneState::EDIT;
                }
                break;
            }
        }
        ImGui::End();
        ImGui::PopStyleVar(3);
        popID();
        return pickedSceneState;
    }

    float height = 30;
};

} // namespace Panda