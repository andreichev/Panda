//
// Created by Andreichev Mikhail on 24.07.2025.
//

#include "PropertiesPanel.hpp"
#include "UI/Properties/AssetProperties/AssetPropertiesDraw.hpp"

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
    int selectedAssetsCount = SelectionContext::selectedAssetsCount();
    World *world = GameContext::getCurrentWorld();
    if (selectedIds.empty() && selectedAssetsCount == 0 || !world) {
        ImGui::Text("No selection");
        ImGui::End();
        return;
    }
    if (!selectedIds.empty() && selectedAssetsCount != 0) {
        ImGui::Text(
            "Selected %ld entities and %ld assets", selectedIds.size(), selectedAssetsCount
        );
    } else if (!selectedIds.empty()) {
        std::unordered_set<Entity> selected = world->getById(selectedIds);
        m_componentsDraw.drawComponents(selected);
    } else if (selectedAssetsCount > 1) {
        ImGui::Text("Selected %ld assets", selectedAssetsCount);
    } else {
        std::unordered_set<path_t> selectedFiles = SelectionContext::getSelectedFiles();
        AssetPropertiesDraw::drawProperties(*selectedFiles.begin());
    }

    ImGui::End();
}

} // namespace Panda
