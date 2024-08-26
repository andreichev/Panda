//
// Created by Michael Andreichev on 20.12.2023.
//

#include "Panels/Common/ImGuiHelper.hpp"
#include "ComponentsDraw.hpp"

#include <Panda/GameLogic/Components/SkyComponent.hpp>
#include <Panda/ImGui/FontAwesome.h>

namespace Panda {

template<typename T>
using UIFunction = void (*)(Entity entity, T &);

template<typename T>
static void
drawComponent(const std::string &name, Entity entity, bool canRemove, UIFunction<T> uiFunction) {
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
        if (canRemove) {
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button(getString(ICON_COG).c_str(), ImVec2(lineHeight, lineHeight))) {
                ImGui::OpenPopup("ComponentSettings");
            }
        }
        bool removeComponent = false;
        if (ImGui::BeginPopup("ComponentSettings")) {
            if (canRemove) {
                if (ImGui::MenuItem("Remove component")) {
                    removeComponent = true;
                }
            }
            ImGui::EndPopup();
        }
        if (open) {
            uiFunction(entity, component);
            ImGui::TreePop();
        }
        if (removeComponent) {
            entity.removeComponent<T>();
        }
        style.IndentSpacing = indentSpacing;
    }
}

static void drawTag(Entity entity) {
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

template<typename T>
static void displayAddComponentEntry(Entity entity, const std::string &entryName) {
    if (!entity.hasComponent<T>()) {
        if (ImGui::MenuItem(entryName.c_str())) {
            entity.addComponent<T>();
            ImGui::CloseCurrentPopup();
        }
    }
}

void ComponentsDraw::displayAddScriptMenuItem(Entity entity) {
    if (ImGui::MenuItem("Add Script")) {
        m_output->addScriptToEntity(entity);
        ImGui::CloseCurrentPopup();
    }
}

ComponentsDraw::ComponentsDraw(Panda::ComponentsDrawOutput *output)
    : m_output(output) {}

void ComponentsDraw::drawComponents(Entity entity) {
    drawTag(entity);
    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("AddComponent");
    }
    if (ImGui::BeginPopup("AddComponent")) {
        displayAddComponentEntry<CameraComponent>(entity, "Camera");
        displayAddComponentEntry<SpriteRendererComponent>(entity, "Sprite Renderer");
        displayAddComponentEntry<SkyComponent>(entity, "Cube Map Rendering");
        displayAddScriptMenuItem(entity);
        ImGui::EndPopup();
    }
    drawComponent<TransformComponent>(
        "Transform",
        entity,
        false,
        [](Entity entity, auto &component) {
            glm::vec3 position = component.getPosition();
            drawVec3Control("Translation", position);
            component.setPosition(position);
            glm::vec3 rotation = component.getRotationEuler();
            drawVec3Control("Rotation", rotation);
            component.setRotationEuler(rotation);
        }
    );
    drawComponent<SpriteRendererComponent>(
        "Sprite Renderer",
        entity,
        true,
        [](Entity entity, auto &component) {
            beginPropertiesGrid();
            propertyColor("Color", component.color);
            endPropertiesGrid();
        }
    );
    drawComponent<DynamicMeshComponent>(
        "Dynamic Mesh",
        entity,
        false,
        [](Entity entity, auto &component) {
            ImGui::Text("Meshes count: %d", (int)component.meshes.size());
        }
    );
    drawComponent<StaticMeshComponent>(
        "Static Mesh",
        entity,
        false,
        [](Entity entity, auto &component) {
            ImGui::Text("Meshes count: %d", (int)component.meshes.size());
        }
    );
    drawComponent<CameraComponent>("Camera", entity, true, [](Entity entity, auto &component) {
        auto &camera = component.camera;
        ImGui::Checkbox("Primary", &component.isPrimary);

        const char *projectionTypeStrings[] = {"Perspective", "Orthographic"};
        const char *currentProjectionTypeString =
            projectionTypeStrings[(int)camera.getProjectionType()];
        if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
            for (int i = 0; i < 2; i++) {
                bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
                    currentProjectionTypeString = projectionTypeStrings[i];
                    camera.setProjectionType((WorldCamera::ProjectionType)i);
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        if (camera.getProjectionType() == WorldCamera::ProjectionType::PERSPECTIVE) {
            float perspectiveVerticalFov = camera.getFieldOfView();
            if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov)) {
                camera.setFieldOfView(perspectiveVerticalFov);
            }
        }
        if (camera.getProjectionType() == WorldCamera::ProjectionType::ORTHOGRAPHIC) {
            float orthoSize = camera.getOrthoSize();
            if (ImGui::DragFloat("Size", &orthoSize)) {
                camera.setOrthoSize(orthoSize);
            }
        }
        float near = camera.getNear();
        if (ImGui::DragFloat("Near", &near)) {
            camera.setNear(near);
        }
        float far = camera.getFar();
        if (ImGui::DragFloat("Far", &far)) {
            camera.setFar(far);
        }
    });
    drawComponent<ScriptListComponent>(
        "Scripts",
        entity,
        false,
        [](Entity entity, auto &component) {
            for (auto &script : component.scripts) {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8);
                ImGui::Text("%s", script.getName().c_str());
                ImGui::SameLine();
                if (ImGui::Button(getString(ICON_TRASH_O).c_str())) {
                    entity.removeScript(script);
                }
            }
            if (component.scripts.empty()) {
                ImGui::Text("No scripts attached to entity");
            } else {
                ImGui::Separator();
                ImGui::Text("%lu scripts", component.scripts.size());
            }
        }
    );
    drawComponent<SkyComponent>(
        "Cube Map Rendering", entity, true, [](Entity entity, auto &component) {}
    );
}

} // namespace Panda
