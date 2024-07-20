//
// Created by Michael Andreichev on 16.09.2023.
//

#include "WorldHierarchyPanel.hpp"
#include "EntityComponents/ComponentsDraw.hpp"

#include <imgui.h>

namespace Panda {

WorldHierarchyPanel::WorldHierarchyPanel(World *world)
    : m_world(world)
    , m_selected() {}

void WorldHierarchyPanel::onImGuiRender() {
    ImGui::Begin("World Hierarchy");
    if (m_world) {
        for (auto entityId : m_world->m_registry.storage<entt::entity>()) {
            Entity entity(&m_world->m_registry, (id_t)entityId, m_world);
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

    ImGui::Begin("Properties");
    if (m_selected.isValid()) {
        drawComponents(m_selected);
    }
    ImGui::End();
}

void WorldHierarchyPanel::drawEntityNode(Entity entity) {
    auto &tag = entity.getComponent<TagComponent>().tag;
    ImGuiTreeNodeFlags flags =
        ((m_selected == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
    bool opened = ImGui::TreeNodeEx((void *)entity.m_id, flags, "%s", tag.c_str());
    if (ImGui::IsItemClicked()) {
        m_selected = entity;
    }
    if (opened) {
        ImGui::TreePop();
    }
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