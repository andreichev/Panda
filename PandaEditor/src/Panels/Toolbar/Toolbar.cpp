#include "Toolbar.hpp"

#include <Panda/GameLogic/GameContext.hpp>

namespace Panda {

Toolbar::Toolbar(ToolbarOutput *output)
    : m_output(output) {}

void Toolbar::onImGuiRender(float offsetY) {
    ImGui::PushID("Editor: Toolbar");
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
    ImGui::SetNextWindowPos({0, offsetY});
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImVec2 size = viewport->WorkSize;
    float width = size.x;
    ImGui::SetNextWindowSize({width, m_height});
    ImGui::Begin(
        "##viewport_central_toolbar", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking
    );
    ImGui::SetCursorPos({200.f, 4.f});
    ImGui::BeginDisabled(m_output->toolbarCanUndo() == false);
    if (ImGui::Button(getString(ICON_UNDO).c_str())) {
        m_output->toolbarUndo();
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::BeginDisabled(m_output->toolbarCanRedo() == false);
    if (ImGui::Button(getString(ICON_REPEAT).c_str())) {
        m_output->toolbarRedo();
    }
    ImGui::EndDisabled();
    ImGui::SetCursorPos({width / 2.f - 70.f, 4.f});
    switch (m_output->toolbarGetCurrentSceneState()) {
        case SceneState::EDIT: {
            ScriptEngine *scriptEngine = GameContext::s_scriptEngine;
            bool scriptsLoaded = scriptEngine && scriptEngine->isLoaded();
            ImGui::BeginDisabled(!scriptsLoaded);
            if (ImGui::Button(getString(ICON_PLAY).c_str())) {
                m_output->toolbarDidPickSceneState(SceneState::PLAY);
            }
            ImGui::SameLine();
            if (ImGui::Button(getString(ICON_PLAY_CIRCLE).c_str())) {
                m_output->toolbarDidPickSceneState(SceneState::SIMULATE);
            }
            ImGui::SameLine();
            ImGui::EndDisabled();
            if (ImGui::Button(getString(ICON_REFRESH).c_str())) {
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
    ImGui::PopStyleVar(4);
    ImGui::PopID();
}

} // namespace Panda