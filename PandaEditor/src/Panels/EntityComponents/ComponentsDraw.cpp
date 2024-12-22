//
// Created by Michael Andreichev on 20.12.2023.
//

#include "Panels/Common/ImGuiHelper.hpp"
#include "ComponentsDraw.hpp"

#include <Panda/GameLogic/WorldCommands/Impl/EntityTransformCommand.hpp>
#include <Panda/GameLogic/WorldCommands/Impl/UpdateSpriteRendererCommand.hpp>
#include <Panda/GameLogic/WorldCommands/Impl/UpdateRigidbody2DCommand.hpp>
#include <Panda/GameLogic/WorldCommands/Impl/UpdateBoxCollider2DCommand.hpp>
#include <Panda/GameLogic/WorldCommands/Impl/AddRemoveComponentCommand.hpp>
#include <Panda/GameLogic/Components/SkyComponent.hpp>
#include <Panda/ImGui/FontAwesome.h>
#include <glm/gtc/type_ptr.hpp>

namespace Panda {

static float coefficientRounding = 3.0f;

template<typename T>
using UIFunction = void (*)(Entity entity, WorldCommandManager &cmd, T &);

template<typename T>
static void
drawComponent(const std::string &name, Entity entity, bool canRemove, UIFunction<T> uiFunction) {
    World *world = entity.getWorld();
    if (!world) {
        return;
    }
    WorldCommandManager &cmd = world->getCommandManger();
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
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f - 3);
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
            uiFunction(entity, cmd, component);
            ImGui::TreePop();
        }
        if (removeComponent) {
            AddRemoveComponentCommand<T> update(entity);
            cmd.DO(update);
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
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, coefficientRounding);
    if (ImGui::InputText("##Tag", buffer, 256)) {
        if (buffer[0] == 0) {
            memcpy(buffer, "Unnamed Entity", 16);
        }
        entity.setName(buffer);
    }
    ImGui::PopStyleVar();
}

template<typename T>
static void
displayAddComponentEntry(Entity entity, WorldCommandManager &cmd, const std::string &entryName) {
    if (!entity.hasComponent<T>()) {
        if (ImGui::MenuItem(entryName.c_str())) {
            AddRemoveComponentCommand<T> update(entity);
            cmd.DO(update);
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
    World *world = entity.getWorld();
    if (!world) {
        return;
    }
    WorldCommandManager &cmd = world->getCommandManger();

    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, coefficientRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);
    drawTag(entity);
    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("AddComponent");
    }
    if (ImGui::BeginPopup("AddComponent")) {
        displayAddComponentEntry<CameraComponent>(entity, cmd, "Camera");
        displayAddComponentEntry<SpriteRendererComponent>(entity, cmd, "Sprite Renderer");
        displayAddComponentEntry<SkyComponent>(entity, cmd, "Cube Map Rendering");
        displayAddComponentEntry<Rigidbody2DComponent>(entity, cmd, "Rigidbody2D");
        displayAddComponentEntry<BoxCollider2DComponent>(entity, cmd, "Box2D Collider");
        displayAddScriptMenuItem(entity);
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);
    drawComponent<TransformComponent>(
        "Transform",
        entity,
        false,
        [](Entity entity, WorldCommandManager &cmd, auto &component) {
            TransformComponent transform = component;
            glm::vec3 position = transform.getPosition();
            if (drawVec3Control("Translation", position)) {
                transform.setPosition(position);
                EntityTransformCommand move(entity, transform);
                cmd.DO(move);
            }
            glm::vec3 rotation = transform.getRotationEuler();
            if (drawVec3Control("Rotation", rotation)) {
                transform.setRotationEuler(rotation);
                EntityTransformCommand rotate(entity, transform);
                cmd.DO(rotate);
            }
            glm::vec3 scale = transform.getScale();
            if (drawVec3Control("Scale", scale)) {
                transform.setScale(scale);
                EntityTransformCommand scale(entity, transform);
                cmd.DO(scale);
            }
        }
    );
    drawComponent<ScriptListComponent>(
        "Scripts",
        entity,
        false,
        [](Entity entity, WorldCommandManager &cmd, auto &component) {
            bool modified = false;
            for (auto &script : component.scripts) {
                shiftCursorY(8);
                ImGui::Text("%s", script.getName().c_str());
                // TODO: Add opening script .cpp file in editor or displaying .cpp file name
                ImGui::SameLine();
                if (ImGui::Button(getString(ICON_TRASH_O).c_str())) {
                    entity.removeScript(script);
                    modified = true;
                    break;
                }
                for (auto &field : script.getFields()) {
                    modified |= drawScriptFieldValue(field);
                }
                underline();
            }
            if (component.scripts.empty()) {
                ImGui::Text("No scripts attached to entity");
            } else {
                ImGui::Separator();
                ImGui::Text("%lu scripts", component.scripts.size());
            }
            if (modified) {
                // TODO: Change script command in history
            }
        }
    );
    drawComponent<SpriteRendererComponent>(
        "Sprite Renderer",
        entity,
        true,
        [](Entity entity, WorldCommandManager &cmd, auto &component) {
            SpriteRendererComponent spriteRenderer = component;
            if (propertyColor("Color", spriteRenderer.color)) {
                UpdateSpriteRendererCommand update(entity, spriteRenderer);
                cmd.DO(update);
            }
            if (propertyTexture("Texture", spriteRenderer.textureId, spriteRenderer.texture)) {
                spriteRenderer.texture = nullptr;
                UpdateSpriteRendererCommand update(entity, spriteRenderer);
                cmd.DO(update);
            }
        }
    );
    drawComponent<DynamicMeshComponent>(
        "Dynamic Mesh",
        entity,
        false,
        [](Entity entity, WorldCommandManager &cmd, auto &component) {
            ImGui::Text("Meshes count: %d", (int)component.meshes.size());
        }
    );
    drawComponent<StaticMeshComponent>(
        "Static Mesh",
        entity,
        false,
        [](Entity entity, WorldCommandManager &cmd, auto &component) {
            ImGui::Text("Meshes count: %d", (int)component.meshes.size());
        }
    );
    drawComponent<CameraComponent>(
        "Camera",
        entity,
        true,
        [](Entity entity, WorldCommandManager &cmd, auto &component) {
            ImGuiStyle &style = ImGui::GetStyle();
            auto &camera = component.camera;
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);
            checkbox("Primary", &component.isPrimary);
            int selectedPos = combo(
                "Projection", {"Perspective", "Orthographic"}, (int)camera.getProjectionType()
            );

            camera.setProjectionType((WorldCamera::ProjectionType)selectedPos);
            if (camera.getProjectionType() == WorldCamera::ProjectionType::PERSPECTIVE) {
                float perspectiveVerticalFov = camera.getFieldOfView();
                if (dragFloat("Vertical FOV", &perspectiveVerticalFov)) {
                    camera.setFieldOfView(perspectiveVerticalFov);
                }
            }
            if (camera.getProjectionType() == WorldCamera::ProjectionType::ORTHOGRAPHIC) {
                float orthoSize = camera.getOrthoSize();
                if (dragFloat("Size", &orthoSize)) {
                    camera.setOrthoSize(orthoSize);
                }
            }
            float near = camera.getNear();
            if (dragFloat("Near", &near)) {
                camera.setNear(near);
            }
            float far = camera.getFar();
            if (dragFloat("Far", &far)) {
                camera.setFar(far);
            }
            ImGui::PopStyleVar();
        }
    );
    drawComponent<Rigidbody2DComponent>(
        "Rigidbody 2D",
        entity,
        true,
        [](Entity entity, WorldCommandManager &cmd, auto &component) {
            const char *bodyTypeStrings[] = {"Static", "Dynamic", "Kinematic"};
            const char *currentBodyTypeString = bodyTypeStrings[(int)component.type];
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0);

            Rigidbody2DComponent rb2d = component;
            int selectedPos =
                combo("Body Type", {"Static", "Dynamic", "Kinematic"}, (int)component.type);
            if (selectedPos != (int)component.type) {
                rb2d.type = (Rigidbody2DComponent::BodyType)selectedPos;
                UpdateRigidbody2DCommand update(entity, rb2d);
                cmd.DO(update);
            }
            if (checkbox("Fixed Rotation", &rb2d.fixedRotation)) {
                UpdateRigidbody2DCommand update(entity, rb2d);
                cmd.DO(update);
            }
            ImGui::PopStyleVar();
        }
    );
    drawComponent<BoxCollider2DComponent>(
        "Box Collider 2D",
        entity,
        true,
        [](Entity entity, WorldCommandManager &cmd, auto &component) {
            BoxCollider2DComponent bc2d = component;
            bool modified = false;
            modified |= dragFloat2("Offset", glm::value_ptr(bc2d.offset));
            modified |= dragFloat("Size", glm::value_ptr(bc2d.size));
            modified |= dragFloat("Density", &bc2d.density, 0.01f, 0.0f, 1.0f);
            modified |= dragFloat("Friction", &bc2d.friction, 0.01f, 0.0f, 1.0f);
            modified |= dragFloat("Restitution", &bc2d.restitution, 0.01f, 0.0f, 1.0f);
            if (modified) {
                UpdateBoxCollider2DCommand update(entity, bc2d);
                cmd.DO(update);
            }
        }
    );
    drawComponent<SkyComponent>(
        "Cube Map Rendering",
        entity,
        true,
        [](Entity entity, WorldCommandManager &cmd, auto &component) {}
    );
}

} // namespace Panda
