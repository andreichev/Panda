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
    : m_componentsDraw(componentsDrawOutput)
    , m_assetPropertiesDraw() {}

void PropertiesPanel::onImGuiRender() {
    ImGui::Begin("Properties");
    std::unordered_set<UUID> selectedIds = SelectionContext::getSelectedEntities();
    std::unordered_set<path_t> selectedAssets = SelectionContext::getSelectedAssets();
    World *world = GameContext::getCurrentWorld();
    if (selectedIds.empty() && selectedAssets.empty() || !world) {
        ImGui::Text("No selection");
        ImGui::End();
        return;
    }
    if (!selectedIds.empty() && !selectedAssets.empty()) {
        ImGui::Text(
            "Selected %ld entities and %ld assets", selectedIds.size(), selectedAssets.size()
        );
    } else if (!selectedIds.empty()) {
        std::unordered_set<Entity> selected = world->getById(selectedIds);
        m_componentsDraw.drawComponents(selected);
    } else if (selectedAssets.size() > 1) {
        ImGui::Text("Selected %ld assets", selectedAssets.size());
    } else {
        m_assetPropertiesDraw.drawProperties(*selectedAssets.begin());
    }

    ImGui::End();
}

} // namespace Panda
