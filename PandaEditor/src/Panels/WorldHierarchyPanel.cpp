//
// Created by Michael Andreichev on 16.09.2023.
//

#include "WorldHierarchyPanel.hpp"
#include "EntityComponents/ComponentsDraw.hpp"

#include <imgui.h>

namespace Panda {

WorldHierarchyPanel::WorldHierarchyPanel(World *world, ComponentsDrawOutput *componentsDrawOutput)
    : m_world(world)
    , m_componentsDraw(componentsDrawOutput)
    , m_selected() {}

void WorldHierarchyPanel::onImGuiRender() {
    ImGui::Begin("World Hierarchy");
    if (m_world && !m_world->isEmpty()) {
        for (auto entityId : m_world->m_registry.storage<entt::entity>()) {
            Entity entity((id_t)entityId, m_world);
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

    ImGui::Begin("Properties");
    if (m_selected.isValid()) {
        m_componentsDraw.drawComponents(m_selected);
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
    if (ImGui::BeginPopupContextItem()) {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
        if (ImGui::Button("Rename", ImVec2(75, ImGui::GetTextLineHeight() + 5))) {

        } else if (ImGui::Button("Delete", ImVec2(75, ImGui::GetTextLineHeight() + 5))) {
        }
        ImGui::PopStyleVar();
        ImGui::EndPopup();
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