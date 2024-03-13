//
// Created by Michael Andreichev on 20.12.2023.
//

#include "Panels/Common/ImGuiHelper.hpp"
#include "ComponentsDraw.hpp"

namespace Panda {

template<typename T>
using UIFunction = void (*)(T &);

template<typename T>
static void drawComponent(const std::string &name, Entity entity, UIFunction<T> uiFunction) {
    const ImGuiTreeNodeFlags treeNodeFlags =
        ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen |
        ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap |
        ImGuiTreeNodeFlags_FramePadding;
    if (entity.hasComponent<T>()) {
        auto &component = entity.getComponent<T>();
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        shiftCursorY(6.f);
        ImGuiStyle &style = ImGui::GetStyle();
        float indentSpacing = style.IndentSpacing;
        style.IndentSpacing = 0.f;
        bool open =
            ImGui::TreeNodeEx((void *)typeid(T).hash_code(), treeNodeFlags, "%s", name.c_str());
        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
        if (ImGui::Button("+", ImVec2(lineHeight, lineHeight))) {
            ImGui::OpenPopup("ComponentSettings");
        }
        bool removeComponent = false;
        if (ImGui::BeginPopup("ComponentSettings")) {
            if (ImGui::MenuItem("Remove component")) {
                removeComponent = true;
            }
            ImGui::EndPopup();
        }
        if (open) {
            uiFunction(component);
            ImGui::TreePop();
        }
        if (removeComponent) {
            entity.removeComponent<T>();
        }
        style.IndentSpacing = indentSpacing;
    }
}

void drawTag(Entity entity) {
    char buffer[256];
    memset(buffer, 0, 256);
    buffer[0] = 0; // Setting the first byte to 0 makes checking if string is empty easier later.
    const std::string &tag = entity.getName();
    memcpy(buffer, tag.c_str(), tag.length());
    if (ImGui::InputText("##Tag", buffer, 256)) {
        if (buffer[0] == 0) {
            memcpy(buffer, "Unnamed Entity", 16);
        }
        entity.getComponent<TagComponent>().tag = buffer;
    }
}

void drawComponents(Entity entity) {
    drawTag(entity);
    drawComponent<TransformComponent>("Transform", entity, [](auto &component) {
        glm::vec3 position = component.getPosition();
        drawVec3Control("Translation", position);
        component.setPosition(position);
        glm::vec3 rotation = component.getRotationEuler();
        glm::vec3 degrees = glm::degrees(rotation);
        drawVec3Control("Rotation", degrees);
        component.setRotationEuler(glm::radians(degrees));
    });
    drawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto &component) {
        beginPropertiesGrid();
        propertyColor("Color", component.color);
        endPropertiesGrid();
    });
}

} // namespace Panda
