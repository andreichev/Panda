//
// Created by Michael Andreichev on 16.09.2023.
//

#include "WorldHierarchyPanel.hpp"
#include "Model/DragDropItem.hpp"
#include "Panda/WorldCommands/Impl/AddRemoveEntitiesCommand.hpp"
#include "Panda/WorldCommands/Impl/HierarchyCommand.hpp"
#include "Panda/ImGui/ImGui+DragTarget.hpp"
#include "Panda/ImGui/ImGui+TreeNode.hpp"

#include <imgui.h>
#include <imgui_internal.h>

namespace Panda {

WorldHierarchyPanel::WorldHierarchyPanel(World *world)
    : m_world(world)
    , m_rowIndex(0)
    , m_firstSelectedRow(-1)
    , m_lastSelectedRow(-1)
    , m_shiftSelectionRunning(false) {}

void WorldHierarchyPanel::onImGuiRender() {
    if (!m_world) { return; }
    ImGuiWindowFlags flags = 0;
    if (m_world->isChanged()) { flags |= ImGuiWindowFlags_UnsavedDocument; }
    ImGui::Begin("World Hierarchy", nullptr, flags);
    m_rowIndex = 0;
    ImRect windowRect = ImGui::GetCurrentWindow()->Rect();
    m_focused = ImGui::IsWindowFocused();
    if (m_world && !m_world->isEmpty()) {
        for (auto entityId : m_world->m_registry.view<entt::entity>()) {
            Entity entity(entityId, m_world);
            if (!entity.isValid()) { continue; }
            if (!entity.getParent()) { drawEntityNode(entity); }
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
    if (m_focused) {
        if (ImGui::IsKeyPressed(ImGuiKey_Backspace, false) ||
            ImGui::IsKeyPressed(ImGuiKey_Delete, false)) {
            WorldCommandManager &cmd = m_world->getCommandManger();
            std::unordered_set<UUID> selectedIds = SelectionContext::getSelectedEntities();
            std::unordered_set<Entity> selected = m_world->getById(selectedIds);
            AddRemoveEntitiesCommand update(selected);
            cmd.SAVE(update, true);
            m_world->setChanged();
            selected = {};
            SelectionContext::unselectAll();
            m_firstSelectedRow = -1;
            m_lastSelectedRow = -1;
        }
    }
    if (ImGui::BeginDragDropTargetCustom(windowRect, ImGui::GetCurrentWindow()->ID)) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(PANDA_DRAGDROP_NAME)) {
            PND_ASSERT(payload->DataSize == sizeof(DragDropItem), "WRONG DRAGDROP ITEM SIZE");
            DragDropItem &item = *(DragDropItem *)payload->Data;
            if (item.type == DragDropItemType::ENTITY) {
                std::vector<Entity> droppedEntities;
                for (int i = 0; i < item.count; i++) {
                    Entity entity = m_world->getById(item.values[i]);
                    if (entity.getParent()) { droppedEntities.push_back(entity); }
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
    ImGui::End();

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) ||
        ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
        if (ImGui::IsMouseHoveringRect(windowRect.Min, windowRect.Max, false) &&
            !ImGui::IsAnyItemHovered() && !ImGui::IsDragDropActive()) {
            m_firstSelectedRow = -1;
            m_lastSelectedRow = -1;
            SelectionContext::unselectAll();
        }
    }
}

void WorldHierarchyPanel::drawEntityNode(Entity entity) {
    // Uncomment if you need to remove bullets
    // float indent = ImGui::GetTreeNodeToLabelSpacing() - 5;
    // ImGui::Unindent(indent);
    UUID entityId = entity.getId();
    m_rowIndex++;
    WorldCommandManager &cmd = m_world->getCommandManger();
    auto &tag = entity.getComponent<TagComponent>().tag;
    ImGuiTreeNodeFlags flags =
        (SelectionContext::isEntitySelected(entityId) ? ImGuiTreeNodeFlags_Selected : 0);
    flags |= ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanFullWidth;
    flags |= ImGuiTreeNodeFlags_FramePadding;
    if (!entity.hasAnyChild()) { flags |= ImGuiTreeNodeFlags_Leaf; }
    void *id = reinterpret_cast<void *>(entity.m_handle);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {2, 3});

    bool isClicked;
    bool opened = ImGui::TreeNodeCustom(ImGui::GetID(id), flags, tag.c_str(), nullptr, &isClicked);
    ImGui::PopStyleVar(2);
    if (m_rowIndex >= m_firstSelectedRow && m_rowIndex <= m_lastSelectedRow &&
        !SelectionContext::isEntitySelected(entityId) && m_shiftSelectionRunning) {
        SelectionContext::addSelectedEntity(entityId);
        if (SelectionContext::selectedEntitiesCount() ==
            (m_lastSelectedRow - m_firstSelectedRow) + 1) {
            m_shiftSelectionRunning = false;
        }
    }
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
        if (ImGui::GetDragDropPayload() == nullptr) {
            DragDropItem item;
            item.type = DragDropItemType::ENTITY;
            if (SelectionContext::isEntitySelected(entityId)) {
                std::vector<UUID> selectedIds;
                for (auto selected : SelectionContext::getSelectedEntities()) {
                    selectedIds.push_back(selected);
                }
                PND_ASSERT_F(
                    selectedIds.size() <= MAX_DRAGDROP_VALUES,
                    "NEED TO INCREASE DragDropItem.data SIZE. REQUIRED %d",
                    selectedIds.size()
                );
                std::copy(selectedIds.begin(), selectedIds.end(), item.values);
                item.count = selectedIds.size();
            } else {
                SelectionContext::unselectAll();
                SelectionContext::addSelectedEntity(entityId);
                item.values[0] = entityId;
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
                std::vector<Entity> droppedEntities;
                for (int i = 0; i < item.count; i++) {
                    // If we didn't drop to the same entity
                    if (item.values[i] == entityId) { continue; }
                    Entity droppedEntity = m_world->getById(item.values[i]);
                    // If entity doesn't have that child already
                    if (entity.hasChild(droppedEntity)) { continue; }
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
        if (shift && SelectionContext::selectedEntitiesCount() > 0) {
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
            SelectionContext::addSelectedEntity(entityId);
            m_firstSelectedRow = m_rowIndex;
            m_lastSelectedRow = -1;
        } else {
            if (SelectionContext::isEntitySelected(entityId)) {
                SelectionContext::removeSelectedEntity(entityId);
            } else {
                SelectionContext::addSelectedEntity(entityId);
            }
        }
        ImGui::FocusWindow(ImGui::GetCurrentWindow());
    }
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5.0f);
    if (ImGui::BeginPopupContextItem()) {
        if (!SelectionContext::isEntitySelected(entityId)) {
            SelectionContext::unselectAll();
            SelectionContext::addSelectedEntity(entityId);
        }
        if (ImGui::MenuItem("Delete", NULL)) {
            std::unordered_set<Entity> entities =
                m_world->getById({SelectionContext::getSelectedEntities()});
            AddRemoveEntitiesCommand update(entities);
            cmd.SAVE(update, true);
            m_world->setChanged();
            SelectionContext::unselectAll();
            m_firstSelectedRow = -1;
            m_lastSelectedRow = -1;
        }
        if (ImGui::MenuItem("Duplicate", NULL)) {
            std::unordered_set<Entity> duplicates;
            for (auto id : SelectionContext::getSelectedEntities()) {
                Entity entity = m_world->getById(id);
                Entity duplicate = m_world->duplicateEntity(entity);
                duplicates.insert(duplicate);
                SelectionContext::removeSelectedEntity(id, false);
                SelectionContext::addSelectedEntity(duplicate.getId(), false);
            }
            AddRemoveEntitiesCommand update(duplicates);
            cmd.SAVE(update, false);
            m_world->setChanged();
            m_world->sort();
            SelectionContext::unselectAll();
            std::unordered_set<UUID> duplicatesIds;
            for (Entity entity : duplicates) {
                duplicatesIds.emplace(entity.getId());
            }
            SelectionContext::addSelectedEntities(duplicatesIds);
            m_firstSelectedRow = -1;
            m_lastSelectedRow = -1;
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
    if (opened) {
        const std::vector<UUID> &children = entity.getChildEntities();
        for (auto child : children) {
            if (!m_world->isValid(child)) { continue; }
            drawEntityNode(m_world->getById(child));
        }
        ImGui::TreePop();
    }
    // ImGui::Indent(indent);
}

void WorldHierarchyPanel::drawEntityCreateMenu() {
    if (!ImGui::BeginMenu("Create")) { return; }
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