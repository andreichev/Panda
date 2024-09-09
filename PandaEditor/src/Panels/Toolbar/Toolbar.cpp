#include "Toolbar.hpp"

namespace Panda {

Toolbar::Toolbar(ToolbarOutput *output)
    : m_output(output) {}

void Toolbar::onImGuiRender(float offsetY) {
    pushID();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::SetNextWindowPos({0, offsetY});
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImVec2 size = viewport->WorkSize;
    float width = size.x;
    ImGui::SetNextWindowSize({width, m_height});
    ImGui::Begin(
        "##viewport_central_toolbar", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking
    );
    ImGui::SetCursorPos({width / 2.f - 70.f, 4.f});
    switch (m_output->toolbarGetCurrentSceneState()) {
        case SceneState::EDIT: {
            if (ImGui::Button(getString(ICON_PLAY).c_str())) {
                m_output->toolbarDidPickSceneState(SceneState::PLAY);
            }
            ImGui::SameLine();
            if (ImGui::Button(getString(ICON_PLAY_CIRCLE).c_str())) {
                m_output->toolbarDidPickSceneState(SceneState::SIMULATE);
            }
            ImGui::SameLine();
            if (ImGui::Button(getString(ICON_CODE).c_str())) {
                m_output->toolbarDidTapReloadScripts();
            }
            break;
        }
        case SceneState::SIMULATE:
        case SceneState::PLAY: {
            if (ImGui::Button(getString(ICON_STOP).c_str())) {
                m_output->toolbarDidPickSceneState(SceneState::EDIT);
            }
            break;
        }
    }
    ImGui::End();
    ImGui::PopStyleVar(3);
    popID();
}

} // namespace Panda