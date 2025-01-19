//
// Created by Michael Andreichev on 16.09.2023.
//

#include "WorldHierarchyPanel.hpp"
#include "EntityComponents/ComponentsDraw.hpp"
#include "Model/DragDropItem.hpp"
#include "Panda/GameLogic/WorldCommands/Impl/AddRemoveEntityCommand.hpp"

#include <imgui.h>

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
    if (m_world && !m_world->isEmpty()) {
        for (auto entityId : m_world->m_registry.view<entt::entity>()) {
            Entity entity(entityId, m_world);
            if (!entity.isValid()) {
                continue;
            }
            drawEntityNode(entity);
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
    ImGui::End();

    Entity selected = m_world->getSelectedEntity();
    ImGui::Begin("Properties");
    if (selected.isValid()) {
        m_componentsDraw.drawComponents(selected);
    }
    ImGui::End();
}

void WorldHierarchyPanel::drawEntityNode(Entity entity) {
    // TODO: Remove unindent when hierarchy is implemented
    float indent = ImGui::GetTreeNodeToLabelSpacing() - 5;
    ImGui::Unindent(indent);
    Entity selected = m_world->getSelectedEntity();
    auto &tag = entity.getComponent<TagComponent>().tag;
    ImGuiTreeNodeFlags flags =
        ((selected == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
    flags |= ImGuiTreeNodeFlags_Leaf;
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
            if (m_world->isSelected(entity)) {
                m_world->setSelectedEntity({});
            }
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
        ImGui::TreePop();
    }
    ImGui::Indent(indent);
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