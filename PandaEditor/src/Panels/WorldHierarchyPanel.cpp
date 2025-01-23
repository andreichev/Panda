//
// Created by Michael Andreichev on 16.09.2023.
//

#include "WorldHierarchyPanel.hpp"
#include "EntityComponents/ComponentsDraw.hpp"
#include "Model/DragDropItem.hpp"
#include "Panda/WorldCommands/Impl/AddRemoveEntityCommand.hpp"
#include "Panda/WorldCommands/Impl/HierarchyCommand.hpp"

#include <imgui.h>
#include <imgui_internal.h>

namespace Panda {

WorldHierarchyPanel::WorldHierarchyPanel(World *world, ComponentsDrawOutput *componentsDrawOutput)
    : m_world(world)
    , m_componentsDraw(componentsDrawOutput) {}

void WorldHierarchyPanel::onImGuiRender() {
    if (!m_world) {
        return;
    }
    ImGuiWindowFlags flags = 0;
    if (m_world->isChanged()) {
        flags |= ImGuiWindowFlags_UnsavedDocument;
    }
    ImGui::Begin("World Hierarchy", nullptr, flags);
    ImRect windowRect = {ImGui::GetWindowContentRegionMin(), ImGui::GetWindowContentRegionMax()};
    m_focused = ImGui::IsWindowFocused();
    if (m_world && !m_world->isEmpty()) {
        for (auto entityId : m_world->m_registry.view<TagComponent>()) {
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
    Entity selected = m_world->getSelectedEntity();
    if (m_focused) {
        if (ImGui::IsKeyPressed(ImGuiKey_Backspace, false) ||
            ImGui::IsKeyPressed(ImGuiKey_Delete, false)) {
            if (selected) {
                WorldCommandManager &cmd = m_world->getCommandManger();
                AddRemoveEntityCommand update(selected);
                cmd.SAVE(update, true);
                m_world->setChanged();
                selected = {};
            }
        }
    }
    if (ImGui::BeginDragDropTargetCustom(windowRect, ImGui::GetCurrentWindow()->ID)) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(PANDA_DRAGDROP_NAME)) {
            PND_ASSERT(payload->DataSize == sizeof(DragDropItem), "WRONG DRAGDROP ITEM SIZE");
            DragDropItem &item = *(DragDropItem *)payload->Data;
            if (item.type == DragDropItemType::ENTITY) {
                UUID droppedId = item.assetId;
                Entity entity = m_world->getById(droppedId);
                WorldCommandManager &cmd = m_world->getCommandManger();
                HierarchyCommand update(entity, {});
                cmd.SAVE(update, true);
            }
        }
        ImGui::EndDragDropTarget();
    }
    ImGui::Begin("Properties");
    if (selected.isValid()) {
        m_componentsDraw.drawComponents(selected);
    }
    ImGui::End();
    ImGui::End();
}

void WorldHierarchyPanel::drawEntityNode(Entity entity) {
    // Uncomment if you need to remove bullets
    // float indent = ImGui::GetTreeNodeToLabelSpacing() - 5;
    // ImGui::Unindent(indent);
    Entity selected = m_world->getSelectedEntity();
    auto &tag = entity.getComponent<TagComponent>().tag;
    ImGuiTreeNodeFlags flags =
        ((selected == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
    if (entity.getChildEntities().empty()) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    void *id = reinterpret_cast<void *>(entity.m_handle);
    bool opened = ImGui::TreeNodeEx(id, flags, "%s", tag.c_str());
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
        DragDropItem item;
        item.type = DragDropItemType::ENTITY;
        item.assetId = entity.getId();
        ImGui::SetDragDropPayload(PANDA_DRAGDROP_NAME, &item, sizeof(DragDropItem));
        ImGui::Text("Entity: %s", entity.getName().c_str());
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(PANDA_DRAGDROP_NAME)) {
            PND_ASSERT(payload->DataSize == sizeof(DragDropItem), "WRONG DRAGDROP ITEM SIZE");
            DragDropItem &item = *(DragDropItem *)payload->Data;
            if (item.type == DragDropItemType::ENTITY) {
                UUID droppedId = item.assetId;
                // If we didn't drop to the same entity
                if (droppedId != entity.getId()) {
                    Entity droppedEntity = m_world->getById(droppedId);
                    WorldCommandManager &cmd = m_world->getCommandManger();
                    HierarchyCommand update(droppedEntity, entity);
                    cmd.SAVE(update, true);
                }
            }
        }
        ImGui::EndDragDropTarget();
    }
    if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        m_world->setSelectedEntity(entity);
    }
    WorldCommandManager &cmd = m_world->getCommandManger();
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5.0f);
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Delete", NULL)) {
            AddRemoveEntityCommand update(entity);
            cmd.SAVE(update, true);
            m_world->setChanged();
        }
        if (ImGui::MenuItem("Duplicate", NULL)) {
            Entity duplicate = m_world->duplicateEntity(entity);
            AddRemoveEntityCommand update(duplicate);
            cmd.SAVE(update, false);
            m_world->setChanged();
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
    if (opened) {
        for (auto child : entity.getChildEntities()) {
            drawEntityNode(m_world->getById(child));
        }
        ImGui::TreePop();
    }
    // ImGui::Indent(indent);
}

void WorldHierarchyPanel::drawEntityCreateMenu() {
    WorldCommandManager &cmd = m_world->getCommandManger();
    if (!ImGui::BeginMenu("Create"))
        return;
    if (ImGui::MenuItem("Empty Entity")) {
        Entity newEntity = m_world->instantiateEntity();
        newEntity.getComponent<TagComponent>().tag = "Empty Entity";
        AddRemoveEntityCommand update(newEntity);
        cmd.SAVE(update, false);
        m_world->setChanged();
    }
    ImGui::EndMenu();
}

} // namespace Panda