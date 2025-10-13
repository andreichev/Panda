//
// Created by Andreichev Mikhail on 03.10.2025.
//

#include "AssetManagerPanel.hpp"

#include "ProjectLoader/AssetHandlerEditor.hpp"
#include "ProjectLoader/ProjectLoader.hpp"
#include "UI/Properties/AssetProperties/AssetPropertiesDraw.hpp"

#include <Panda/GameLogic/SelectionContext.hpp>
#include <Panda/GameLogic/GameContext.hpp>
#include <imgui.h>

namespace Panda {

AssetManagerPanel::AssetManagerPanel(ProjectLoader *projectLoader)
    : m_projectLoader(projectLoader) {}

void AssetManagerPanel::drawAssetRow(AssetInfo assetInfo) {
    AssetHandlerEditor *assetHandler =
        static_cast<AssetHandlerEditor *>(GameContext::getAssetHandler());
    bool ctrl = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl);
    bool shift = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift);
    uint32_t id = assetInfo.id;
    char label[32];
    sprintf(label, "%u", id);
    ImGui::PushID(id);
    ImGui::TableNextRow();
    bool isValid = isAssetValid(assetInfo);
    bool isSelected = isAssetSelected(assetInfo);
    if (!isValid) { ImGui::PushStyleColor(ImGuiCol_Text, 0xFF0000FF); }
    // ID
    ImGui::TableNextColumn();
    ImGuiSelectableFlags selectableFlags =
        ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap;
    if (ImGui::Selectable(label, isSelected, selectableFlags)) {
        if (!shift && !ctrl) { SelectionContext::unselectAll(); }
        std::unordered_set<path_t> paths = assetHandler->getAssetPaths(id);
        SelectionContext::addSelectedFiles(paths);
    }
    // TYPE
    ImGui::TableNextColumn();
    ImGui::Text("%d", assetInfo.type);
    // NAME
    ImGui::TableNextColumn();
    ImGui::Text("Name");
    // IS VALID
    ImGui::TableNextColumn();
    ImGui::Text("%d", isValid);
    // IS LOADED
    ImGui::TableNextColumn();
    ImGui::Text("%d", false);
    if (!isValid) { ImGui::PopStyleColor(); }
    ImGui::PopID();
}

void AssetManagerPanel::onImGuiRender() {
    AssetHandlerEditor *assetHandler =
        static_cast<AssetHandlerEditor *>(GameContext::getAssetHandler());
    auto registry = assetHandler->getRegistry();

    ImGui::Begin("Asset Manager", nullptr);
    ImGuiTableFlags flags = ImGuiTableFlags_Sortable | ImGuiTableFlags_Borders;
    // 1) ID
    // 2) TYPE
    // 3) NAME
    // 4) IS_VALID
    // *) [CREATION_DATE]
    // 5) IS_LOADED
    if (ImGui::BeginTable("Asset Manager", 5, flags)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("TYPE");
        ImGui::TableSetupColumn("NAME");
        ImGui::TableSetupColumn("IS VALID");
        ImGui::TableSetupColumn("IS LOADED");
        ImGui::TableHeadersRow();
        // if (ImGuiTableSortSpecs* sortSpecs = ImGui::TableGetSortSpecs())
        //     if (sortSpecs->SpecsDirty || RequestSort) {
        //         ExampleAsset::SortWithSortSpecs(sort_specs, Items.Data, Items.Size);
        //         sort_specs->SpecsDirty = RequestSort = false;
        //     }
        for (auto [_, assetInfo] : registry) {
            drawAssetRow(assetInfo);
        }
        ImGui::EndTable();
    }
    ImGui::End();
}

bool AssetManagerPanel::isAssetValid(const AssetInfo &info) {
    const path_t &projectPath = m_projectLoader->getOpenedProjectPath();
    switch (info.type) {
        case AssetType::TEXTURE: {
            TextureAssetMeta meta = std::get<TextureAssetMeta>(info.meta);
            if (!std::filesystem::exists(projectPath / meta.path)) { return false; }
            break;
        }
        case AssetType::SHADER: {
            ShaderAssetMeta meta = std::get<ShaderAssetMeta>(info.meta);
            if (!std::filesystem::exists(projectPath / meta.vertexCodePath)) { return false; }
            if (!std::filesystem::exists(projectPath / meta.fragmentCodePath)) { return false; }
            break;
        }
        case AssetType::MATERIAL: {
            MaterialAssetMeta meta = std::get<MaterialAssetMeta>(info.meta);
            if (!std::filesystem::exists(projectPath / meta.materialPath)) { return false; }
            break;
        }
        case AssetType::CUBE_MAP:
        case AssetType::MESH:
        case AssetType::NONE: {
            break;
        }
    }
    return true;
}

bool AssetManagerPanel::isAssetSelected(const AssetInfo &info) {
    AssetHandlerEditor *assetHandler =
        static_cast<AssetHandlerEditor *>(GameContext::getAssetHandler());
    std::unordered_set<path_t> paths = assetHandler->getAssetPaths(info.id);
    for (auto &path : paths) {
        if (SelectionContext::isFileSelected(path)) { return true; }
    }
    return false;
}

} // namespace Panda