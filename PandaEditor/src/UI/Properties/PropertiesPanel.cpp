//
// Created by Andreichev Mikhail on 24.07.2025.
//

#include "PropertiesPanel.hpp"

#include <Panda/Base/Base.hpp>
#include <Panda/GameLogic/SelectionContext.hpp>
#include <imgui.h>
#include <unordered_set>

namespace Panda {

PropertiesPanel::PropertiesPanel(ComponentsDrawOutput *componentsDrawOutput)
    : m_componentsDraw(componentsDrawOutput) {}

void PropertiesPanel::onImGuiRender() {
    ImGui::Begin("Properties");
    std::unordered_set<UUID> selectedIds = SelectionContext::getSelectedEntities();
    World *world = GameContext::s_currentWorld;
    if (selectedIds.empty() || !world) {
        ImGui::End();
        return;
    }
    std::unordered_set<Entity> selected = world->getById(selectedIds);
    m_componentsDraw.drawComponents(selected);
    ImGui::End();
}

} // namespace Panda
