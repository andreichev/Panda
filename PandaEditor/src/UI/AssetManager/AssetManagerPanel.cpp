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
#include <imgui_internal.h>

namespace Panda {

AssetManagerPanel::AssetManagerPanel(ProjectLoader *projectLoader)
    : m_projectLoader(projectLoader)
    , m_rowIndex(0)
    , m_firstSelectedRow(-1)
    , m_lastSelectedRow(-1)
    , m_shiftSelectionRunning(false) {}

void AssetManagerPanel::drawAssetRow(AssetInfo assetInfo) {
    AssetHandlerEditor *assetHandler =
        static_cast<AssetHandlerEditor *>(GameContext::getAssetHandler());
    bool ctrl = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl);
    bool shift = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift);
    uint32_t id = assetInfo.id;
    char label[32];
    snprintf(label, 30, "%u", id);
    ImGui::PushID(id);
    ImGui::TableNextRow();
    bool isValid = assetHandler->assetFilesExist(id);
    bool isLoaded = assetHandler->isLoaded(id);
    bool isSelected = isAssetSelected(assetInfo);
    if (!isValid) { ImGui::PushStyleColor(ImGuiCol_Text, 0xFF0000FF); }
    // ID
    ImGui::TableNextColumn();
    ImGuiSelectableFlags selectableFlags =
        ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap;
    bool isClicked = ImGui::Selectable(label, isSelected, selectableFlags);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5.0f);
    if (ImGui::BeginPopupContextItem(label)) {
        if (!isSelected) {
            SelectionContext::unselectAll();
            selectAsset(id);
        }
        if (ImGui::MenuItem("Delete", NULL)) {
            deleteSelectedAssets();
            SelectionContext::unselectAll();
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
    if (m_rowIndex >= m_firstSelectedRow && m_rowIndex <= m_lastSelectedRow && !isSelected &&
        m_shiftSelectionRunning) {
        selectAsset(id);
        if (SelectionContext::selectedAssetsCount() ==
            (m_lastSelectedRow - m_firstSelectedRow) + 1) {
            m_shiftSelectionRunning = false;
        }
    }
    // TYPE
    ImGui::TableNextColumn();
    ImGui::TextUnformatted(assetInfo.getTypeStr());
    // NAME
    ImGui::TableNextColumn();
    ImGui::TextUnformatted(getAssetName(assetInfo).c_str());
    // IS VALID
    ImGui::TableNextColumn();
    ImGui::Text("%d", isValid);
    // IS LOADED
    ImGui::TableNextColumn();
    ImGui::Text("%d", isLoaded);
    if (!isValid) { ImGui::PopStyleColor(); }

    if (isClicked) {
        if (shift && SelectionContext::selectedAssetsCount() > 0) {
            SelectionContext::unselectAll();
            if (m_rowIndex < m_firstSelectedRow) {
                m_lastSelectedRow = m_firstSelectedRow;
                m_firstSelectedRow = m_rowIndex;
            } else {
                m_lastSelectedRow = m_rowIndex;
            }
            m_shiftSelectionRunning = true;
        } else if (!ctrl || shift) {
            SelectionContext::unselectAll();
            selectAsset(id);
            m_firstSelectedRow = m_rowIndex;
            m_lastSelectedRow = -1;
        } else {
            if (SelectionContext::isAssetSelected(id)) {
                unselectAsset(id);
            } else {
                selectAsset(id);
            }
        }
        ImGui::FocusWindow(ImGui::GetCurrentWindow());
    }
    ImGui::PopID();
}

void AssetManagerPanel::onImGuiRender() {
    AssetHandlerEditor *assetHandler =
        static_cast<AssetHandlerEditor *>(GameContext::getAssetHandler());
    auto registry = assetHandler->getRegistry();

    ImGui::Begin("Asset Manager", nullptr);
    ImGuiTableFlags flags = ImGuiTableFlags_Borders;
    // 1) ID
    // 2) TYPE
    // 3) NAME
    // 4) IS_VALID
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
        m_rowIndex = 0;
        for (auto [_, assetInfo] : registry) {
            drawAssetRow(assetInfo);
            m_rowIndex++;
        }
        ImGui::EndTable();
    }
    ImGui::End();
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

void AssetManagerPanel::selectAsset(AssetId id) {
    auto assetHandler = GameContext::getAssetHandler();
    std::unordered_set<path_t> paths = assetHandler->getAssetPaths(id);
    SelectionContext::addSelectedFiles(paths);
}

void AssetManagerPanel::unselectAsset(AssetId id) {
    SelectionContext::removeSelectedAsset(id);
}

void AssetManagerPanel::deleteSelectedAssets() {
    AssetHandlerEditor *assetHandler =
        static_cast<AssetHandlerEditor *>(GameContext::getAssetHandler());
    auto selectedFiles = SelectionContext::getSelectedFiles();
    for (auto &file : selectedFiles) {
        AssetId assetId = assetHandler->getAssetId(file);
        if (assetId) { assetHandler->removeAsset(assetId); }
    }
}

std::string AssetManagerPanel::getAssetName(const AssetInfo &info) {
    switch (info.type) {
        case AssetType::TEXTURE: {
            TextureAssetMeta meta = std::get<TextureAssetMeta>(info.meta);
            return meta.path.filename().string();
        }
        case AssetType::SHADER: {
            ShaderAssetMeta meta = std::get<ShaderAssetMeta>(info.meta);
            std::string filename = meta.vertexCodePath.filename().string() + " " +
                                   meta.fragmentCodePath.filename().string();
            return filename;
            break;
        }
        case AssetType::MATERIAL: {
            MaterialAssetMeta meta = std::get<MaterialAssetMeta>(info.meta);
            return meta.materialPath.filename().string();
            break;
        }
        case AssetType::CUBE_MAP:
        case AssetType::MESH:
        case AssetType::NONE: {
            return {};
        }
    }
}

} // namespace Panda