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
        m_world->m_registry.each([&](auto entityId) {
            Entity entity(&m_world->m_registry, (id_t)entityId, m_world);
            drawEntityNode(entity);
        });
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

} // namespace Panda