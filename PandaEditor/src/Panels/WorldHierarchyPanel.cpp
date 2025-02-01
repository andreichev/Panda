//
// Created by Michael Andreichev on 16.09.2023.
//

#include "WorldHierarchyPanel.hpp"
#include "EntityComponents/ComponentsDraw.hpp"
#include "Model/DragDropItem.hpp"
#include "Panda/WorldCommands/Impl/AddRemoveEntitiesCommand.hpp"
#include "Panda/WorldCommands/Impl/HierarchyCommand.hpp"
#include "Panda/ImGui/ImGui+DragTarget.hpp"
#include "Panda/ImGui/ImGui+TreeNode.hpp"

#include <imgui.h>
#include <imgui_internal.h>

namespace Panda {

WorldHierarchyPanel::WorldHierarchyPanel(World *world, ComponentsDrawOutput *componentsDrawOutput)
    : m_world(world)
    , m_rowIndex(0)
    , m_componentsDraw(componentsDrawOutput)
    , m_firstSelectedRow(-1)
    , m_lastSelectedRow(-1)
    , m_shiftSelectionRunning(false) {}

void WorldHierarchyPanel::onImGuiRender() {
    if (!m_world) {
        return;
    }
    ImGuiWindowFlags flags = 0;
    if (m_world->isChanged()) {
        flags |= ImGuiWindowFlags_UnsavedDocument;
    }
    ImGui::Begin("World Hierarchy", nullptr, flags);
    m_rowIndex = 0;
    ImRect windowRect = {ImGui::GetWindowContentRegionMin(), ImGui::GetWindowContentRegionMax()};
    m_focused = ImGui::IsWindowFocused();
    if (m_world && !m_world->isEmpty()) {
        for (auto entityId : m_world->m_registry.view<entt::entity>()) {
            Entity entity(entityId, m_world);
            if (!entity.isValid()) {
                continue;
            }
            if (!entity.getParent()) {
                drawEntityNode(entity);
            }
        }
    }
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5.0f);
    if (ImGui::BeginPopupContextWindow(
            nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems
        )) {
        drawEntityCreateMenu();
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
    SelectionContext &selectionContext = m_world->getSelectionContext();
    std::vector<Entity> selected = selectionContext.getSelectedEntities();
    if (m_focused) {
        if (ImGui::IsKeyPressed(ImGuiKey_Backspace, false) ||
            ImGui::IsKeyPressed(ImGuiKey_Delete, false)) {
            WorldCommandManager &cmd = m_world->getCommandManger();
            AddRemoveEntitiesCommand update(selected);
            cmd.SAVE(update, true);
            m_world->setChanged();
            selected = {};
            selectionContext.unselectAll();
            m_firstSelectedRow = -1;
            m_lastSelectedRow = -1;
        }
    }
    if (ImGui::BeginDragDropTargetCustom(windowRect, ImGui::GetCurrentWindow()->ID)) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(PANDA_DRAGDROP_NAME)) {
            PND_ASSERT(payload->DataSize == sizeof(DragDropItem), "WRONG DRAGDROP ITEM SIZE");
            DragDropItem &item = *(DragDropItem *)payload->Data;
            if (item.type == DragDropItemType::ENTITY) {
                UUID *droppedIds = (UUID *)item.data;
                std::vector<Entity> droppedEntities;
                for (int i = 0; i < item.count; i++) {
                    Entity entity = m_world->getById(droppedIds[i]);
                    if (entity.getParent()) {
                        droppedEntities.push_back(entity);
                    }
                }
                if (!droppedEntities.empty()) {
                    WorldCommandManager &cmd = m_world->getCommandManger();
                    HierarchyCommand update(droppedEntities, {});
                    cmd.SAVE(update, true);
                }
            }
        }
        ImGui::EndDragDropTarget();
    }
    ImGui::Begin("Properties");
    if (!selected.empty()) {
        if (selected.size() == 1) {
            m_componentsDraw.drawComponents(selected[0]);
        } else {
            ImGui::Text("Multiple edit not supported yet");
        }
    }
    ImGui::End();
    ImGui::End();

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) ||
        ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
        if (ImGui::IsMouseHoveringRect(windowRect.Min, windowRect.Max, false) &&
            !ImGui::IsAnyItemHovered()) {
            m_firstSelectedRow = -1;
            m_lastSelectedRow = -1;
            selectionContext.unselectAll();
        }
    }
}

void WorldHierarchyPanel::drawEntityNode(Entity entity) {
    // Uncomment if you need to remove bullets
    // float indent = ImGui::GetTreeNodeToLabelSpacing() - 5;
    // ImGui::Unindent(indent);
    m_rowIndex++;
    SelectionContext &selectionContext = m_world->getSelectionContext();
    WorldCommandManager &cmd = m_world->getCommandManger();
    auto &tag = entity.getComponent<TagComponent>().tag;
    ImGuiTreeNodeFlags flags =
        (selectionContext.isSelected(entity) ? ImGuiTreeNodeFlags_Selected : 0);
    flags |= ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanFullWidth;
    flags |= ImGuiTreeNodeFlags_FramePadding;
    if (!entity.hasAnyChild()) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    void *id = reinterpret_cast<void *>(entity.m_handle);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {2, 3});

    bool isClicked;
    bool opened = ImGui::TreeNodeCustom(ImGui::GetID(id), flags, tag.c_str(), nullptr, &isClicked);
    ImGui::PopStyleVar(2);
    if (m_rowIndex >= m_firstSelectedRow && m_rowIndex <= m_lastSelectedRow &&
        !selectionContext.isSelected(entity) && m_shiftSelectionRunning) {
        selectionContext.addSelectedEntity(entity);
        if (selectionContext.selectedCount() == (m_lastSelectedRow - m_firstSelectedRow) + 1) {
            m_shiftSelectionRunning = false;
        }
    }
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
        if (ImGui::GetDragDropPayload() == nullptr) {
            DragDropItem item;
            item.type = DragDropItemType::ENTITY;
            if (selectionContext.isSelected(entity)) {
                std::vector<UUID> selectedIds;
                for (auto selected : selectionContext.getSelectedEntities()) {
                    selectedIds.push_back(selected.getId());
                }
                PND_ASSERT_F(
                    sizeof(UUID) * selectedIds.size() <= sizeof(DragDropItem::data),
                    "NEED TO INCREASE DragDropItem.data SIZE. REQUIRED {}",
                    sizeof(UUID) * selectedIds.size()
                );
                memcpy(item.data, selectedIds.data(), sizeof(UUID) * selectedIds.size());
                item.count = selectionContext.selectedCount();
            } else {
                selectionContext.unselectAll();
                selectionContext.addSelectedEntity(entity);
                UUID assetId = entity.getId();
                PND_STATIC_ASSERT(sizeof(assetId) <= sizeof(DragDropItem::data));
                memcpy(item.data, &assetId, sizeof(assetId));
                item.count = 1;
            }
            ImGui::SetDragDropPayload(PANDA_DRAGDROP_NAME, &item, sizeof(DragDropItem));
        }
        DragDropItem *item = (DragDropItem *)ImGui::GetDragDropPayload()->Data;
        if (item->count == 1) {
            ImGui::Text("Entity: %s", entity.getName().c_str());
        } else {
            ImGui::Text("%d entities", item->count);
        }
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTargetCustom(true)) {
        // TO REMOVE YELLOW OUTLINE: ImGuiDragDropFlags_AcceptNoDrawDefaultRect
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(PANDA_DRAGDROP_NAME)) {
            PND_ASSERT(payload->DataSize == sizeof(DragDropItem), "WRONG DRAGDROP ITEM SIZE");
            DragDropItem &item = *(DragDropItem *)payload->Data;
            if (item.type == DragDropItemType::ENTITY) {
                UUID *droppedIds = (UUID *)item.data;
                std::vector<Entity> droppedEntities;
                for (int i = 0; i < item.count; i++) {
                    // If we didn't drop to the same entity
                    if (droppedIds[i] == entity.getId()) {
                        continue;
                    }
                    Entity droppedEntity = m_world->getById(droppedIds[i]);
                    // If entity doesn't have that child already
                    if (entity.hasChild(droppedEntity)) {
                        continue;
                    }
                    droppedEntities.push_back(droppedEntity);
                }
                if (!droppedEntities.empty()) {
                    HierarchyCommand update(droppedEntities, entity);
                    cmd.SAVE(update, true);
                }
            }
        }
        ImGui::EndDragDropTarget();
    }

    if (isClicked && !ImGui::IsItemToggledOpen()) {
        bool ctrl = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl);
        bool shift = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift);
        if (shift && selectionContext.selectedCount() > 0) {
            selectionContext.unselectAll();
            if (m_rowIndex < m_firstSelectedRow) {
                m_lastSelectedRow = m_firstSelectedRow;
                m_firstSelectedRow = m_rowIndex;
            } else {
                m_lastSelectedRow = m_rowIndex;
            }
            m_shiftSelectionRunning = true;
        } else if (!ctrl || shift) {
            selectionContext.unselectAll();
            selectionContext.addSelectedEntity(entity);
            m_firstSelectedRow = m_rowIndex;
            m_lastSelectedRow = -1;
        } else {
            if (selectionContext.isSelected(entity)) {
                selectionContext.removeSelectedEntity(entity);
            } else {
                selectionContext.addSelectedEntity(entity);
            }
        }
        ImGui::FocusWindow(ImGui::GetCurrentWindow());
    }
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5.0f);
    if (ImGui::BeginPopupContextItem()) {
        if (!selectionContext.isSelected(entity)) {
            selectionContext.unselectAll();
            selectionContext.addSelectedEntity(entity);
        }
        if (ImGui::MenuItem("Delete", NULL)) {
            AddRemoveEntitiesCommand update({selectionContext.getSelectedEntities()});
            cmd.SAVE(update, true);
            m_world->setChanged();
            selectionContext.unselectAll();
            m_firstSelectedRow = -1;
            m_lastSelectedRow = -1;
        }
        if (ImGui::MenuItem("Duplicate", NULL)) {
            std::vector<Entity> duplicates;
            for (auto entity : selectionContext.getSelectedEntities()) {
                Entity duplicate = m_world->duplicateEntity(entity);
                duplicates.push_back(duplicate);
            }
            AddRemoveEntitiesCommand update(duplicates);
            cmd.SAVE(update, false);
            m_world->setChanged();
            m_world->sort();
            selectionContext.unselectAll();
            selectionContext.addSelectedEntities(duplicates);
            m_firstSelectedRow = -1;
            m_lastSelectedRow = -1;
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
    if (opened) {
        const std::vector<UUID> &children = entity.getChildEntities();
        for (auto child : children) {
            if (!m_world->isValid(child)) {
                continue;
            }
            drawEntityNode(m_world->getById(child));
        }
        ImGui::TreePop();
    }
    // ImGui::Indent(indent);
}

void WorldHierarchyPanel::drawEntityCreateMenu() {
    if (!ImGui::BeginMenu("Create")) {
        return;
    }
    if (ImGui::MenuItem("Empty Entity")) {
        Entity newEntity = m_world->instantiateEntity();
        newEntity.getComponent<TagComponent>().tag = "Empty Entity";
        WorldCommandManager &cmd = m_world->getCommandManger();
        AddRemoveEntitiesCommand update({newEntity});
        cmd.SAVE(update, false);
        m_world->setChanged();
    }
    ImGui::EndMenu();
}

} // namespace Panda