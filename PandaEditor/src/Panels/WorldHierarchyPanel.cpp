//
// Created by Michael Andreichev on 16.09.2023.
//

#include "WorldHierarchyPanel.hpp"
#include "EntityComponents/ComponentsDraw.hpp"
#include "Model/DragDropItem.hpp"

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
        for (auto entityId : m_world->m_registry.storage<entt::entity>()) {
            Entity entity(entityId, m_world);
            if (!entity.isValid()) {
                continue;
            }
            drawEntityNode(entity);
        }
    }
    if (ImGui::BeginPopupContextWindow(
            nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems
        )) {
        drawEntityCreateMenu();
        ImGui::EndPopup();
    }
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
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Delete", NULL)) {
            m_world->destroy(entity);
        }
        ImGui::EndPopup();
    }
    if (opened) {
        ImGui::TreePop();
    }
    ImGui::Indent(indent);
}

void WorldHierarchyPanel::drawEntityCreateMenu() {
    if (!ImGui::BeginMenu("Create"))
        return;
    if (ImGui::MenuItem("Empty Entity")) {
        Entity newEntity = m_world->instantiateEntity();
        newEntity.getComponent<TagComponent>().tag = "Empty Entity";
    }
    ImGui::EndMenu();
}

} // namespace Panda