//
// Created by Michael Andreichev on 20.12.2023.
//

#include "UI/Common/ImGuiHelper.hpp"
#include "UI/Properties/EntityComponents/ComponentsDraw.hpp"
#include "UI/Properties/EntityComponents/ConsistensyCheck.hpp"

#include <Panda/WorldCommands/Impl/EntityTransformCommand.hpp>
#include <Panda/WorldCommands/Impl/UpdateSpriteRendererCommand.hpp>
#include <Panda/WorldCommands/Impl/UpdateRigidbody2DCommand.hpp>
#include <Panda/WorldCommands/Impl/UpdateBoxCollider2DCommand.hpp>
#include <Panda/WorldCommands/Impl/AddRemoveComponentCommand.hpp>
#include <Panda/GameLogic/Components/SkyComponent.hpp>
#include <Panda/GameLogic/GameContext.hpp>
#include <Panda/ImGui/FontAwesome.h>
#include <glm/gtc/type_ptr.hpp>

namespace Panda {

static float coefficientRounding = 3.0f;

template<typename T>
using UIFunction = std::function<void(std::unordered_set<Entity>, WorldCommandManager &, T &)>;

template<typename T>
static void drawComponent(
    const std::string &name,
    const std::unordered_set<Entity> &entities,
    bool canRemove,
    UIFunction<T> uiFunction
) {
    if (entities.empty()) { return; }
    Entity firstEntity = *entities.begin();
    World *world = firstEntity.getWorld();
    if (!world) { return; }
    WorldCommandManager &cmd = world->getCommandManger();
    const ImGuiTreeNodeFlags treeNodeFlags =
        ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen |
        ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap |
        ImGuiTreeNodeFlags_FramePadding;
    bool allEntitiesHaveComponent = true;
    for (Entity entity : entities) {
        if (!entity.hasComponent<T>()) {
            allEntitiesHaveComponent = false;
            break;
        }
    }
    if (!allEntitiesHaveComponent) { return; }
    auto &firstComponent = firstEntity.getComponent<T>();
    ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    shiftCursorY(6.f);
    ImGuiStyle &style = ImGui::GetStyle();
    float indentSpacing = style.IndentSpacing;
    style.IndentSpacing = 0.f;
    bool open = ImGui::TreeNodeEx((void *)typeid(T).hash_code(), treeNodeFlags, "%s", name.c_str());
    if (canRemove) {
        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f - 3);
        if (ImGui::Button(getString(ICON_COG).c_str(), ImVec2(lineHeight, lineHeight))) {
            ImGui::OpenPopup("ComponentSettings");
        }
    }
    bool removeComponent = false;
    if (ImGui::BeginPopup("ComponentSettings")) {
        if (canRemove) {
            if (ImGui::MenuItem("Remove component")) { removeComponent = true; }
        }
        ImGui::EndPopup();
    }
    if (open) {
        uiFunction(entities, cmd, firstComponent);
        ImGui::TreePop();
    }
    if (removeComponent) {
        AddRemoveComponentCommand<T> update(entities);
        cmd.SAVE(update);
    }
    style.IndentSpacing = indentSpacing;
}

static void drawTag(const std::unordered_set<Entity> &entities) {
    if (entities.empty()) { return; }
    Entity firstEntity = *entities.begin();
    const bool inconsistentTag = isInconsistentPrimitive<std::string, TagComponent>(
        entities, [](const TagComponent &tagComponent) { return tagComponent.tag; }
    );
    char buffer[256];
    memset(buffer, 0, 256);
    buffer[0] = 0; // Setting the first byte to 0 makes checking if string is empty easier later.
    const std::string &tag = inconsistentTag ? "---" : firstEntity.getName();
    memcpy(buffer, tag.c_str(), tag.length());
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, coefficientRounding);
    if (ImGui::InputText("##Tag", buffer, 256)) {
        if (buffer[0] == 0) { memcpy(buffer, "Unnamed Entity", 16); }
        for (Entity entity : entities) {
            entity.setName(buffer);
        }
        firstEntity.sortWorld();
    }
    ImGui::PopStyleVar();
}

template<typename T>
static void displayAddComponentEntry(
    const std::unordered_set<Entity> &entities,
    WorldCommandManager &cmd,
    const std::string &entryName
) {
    bool allEntitiesHaveComponent = true;
    for (Entity entity : entities) {
        if (!entity.hasComponent<T>()) {
            allEntitiesHaveComponent = false;
            break;
        }
    }
    if (!allEntitiesHaveComponent) {
        if (ImGui::MenuItem(entryName.c_str())) {
            AddRemoveComponentCommand<T> update(entities);
            cmd.SAVE(update);
            ImGui::CloseCurrentPopup();
        }
    }
}

void ComponentsDraw::displayAddScriptMenuItem(const std::unordered_set<Entity> &entities) {
    if (ImGui::MenuItem("Add Script")) {
        m_output->addScriptToEntities(entities);
        ImGui::CloseCurrentPopup();
    }
}

ComponentsDraw::ComponentsDraw(Panda::ComponentsDrawOutput *output)
    : m_output(output) {}

void ComponentsDraw::drawComponents(const std::unordered_set<Entity> &entities) {
    if (entities.empty()) { return; }
    const bool isMultiSelect = entities.size() > 1;
    Entity firstEntity = *entities.begin();
    World *world = firstEntity.getWorld();
    if (!world) { return; }
    WorldCommandManager &cmd = world->getCommandManger();

    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, coefficientRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);
    drawTag(entities);
    if (ImGui::Button("Add Component")) { ImGui::OpenPopup("AddComponent"); }
    if (ImGui::BeginPopup("AddComponent")) {
        displayAddComponentEntry<CameraComponent>(entities, cmd, "Camera");
        displayAddComponentEntry<SpriteRendererComponent>(entities, cmd, "Sprite Renderer");
        displayAddComponentEntry<SkyComponent>(entities, cmd, "Cube Map Rendering");
        displayAddComponentEntry<Rigidbody2DComponent>(entities, cmd, "Rigidbody2D");
        displayAddComponentEntry<BoxCollider2DComponent>(entities, cmd, "Box2D Collider");
        displayAddScriptMenuItem(entities);
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);
    drawComponent<TransformComponent>(
        "Transform",
        entities,
        false,
        [&](std::unordered_set<Entity> entities, WorldCommandManager &cmd, auto &firstComponent) {
            glm::vec3 position = firstComponent.getPosition();
            glm::vec3 rotation = firstComponent.getRotationEuler();
            glm::vec3 scale = firstComponent.getScale();
            glm::vec3 oldPosition = position;
            glm::vec3 oldRotation = rotation;
            glm::vec3 oldScale = scale;
            bool modified = false;
            uint32_t translationAxes =
                getInconsistentAxes(entities, [](const TransformComponent &c) {
                    return c.getPosition();
                });
            modified |= drawVec3Control("Translation", position, 0.0, translationAxes);
            uint32_t rotationAxes = getInconsistentAxes(entities, [](const TransformComponent &c) {
                return c.getRotationEuler();
            });
            modified |= drawVec3Control("Rotation", rotation, 0.0, rotationAxes);
            uint32_t scaleAxes = getInconsistentAxes(entities, [](const TransformComponent &c) {
                return c.getScale();
            });
            modified |= drawVec3Control("Scale", scale, 1.0, scaleAxes);
            if (modified) {
                EntityTransformCommand command(entities);
                command.saveBeforeEdit();
                translationAxes = getInconsistentAxes(oldPosition, position);
                rotationAxes = getInconsistentAxes(oldRotation, rotation);
                scaleAxes = getInconsistentAxes(oldScale, scale);
                for (Entity entity : entities) {
                    auto &component = entity.getTransform();
                    glm::vec3 _pos = component.getPosition();
                    glm::vec3 _rot = component.getRotationEuler();
                    glm::vec3 _scale = component.getScale();
                    if (translationAxes & (uint32_t)VectorAxes::X) { _pos.x = position.x; }
                    if (translationAxes & (uint32_t)VectorAxes::Y) { _pos.y = position.y; }
                    if (translationAxes & (uint32_t)VectorAxes::Z) { _pos.z = position.z; }
                    if (rotationAxes & (uint32_t)VectorAxes::X) { _rot.x = rotation.x; }
                    if (rotationAxes & (uint32_t)VectorAxes::Y) { _rot.y = rotation.y; }
                    if (rotationAxes & (uint32_t)VectorAxes::Z) { _rot.z = rotation.z; }
                    if (scaleAxes & (uint32_t)VectorAxes::X) { _scale.x = scale.x; }
                    if (scaleAxes & (uint32_t)VectorAxes::Y) { _scale.y = scale.y; }
                    if (scaleAxes & (uint32_t)VectorAxes::Z) { _scale.z = scale.z; }
                    component.setPosition(_pos);
                    component.setRotationEuler(_rot);
                    component.setScale(_scale);
                }
                command.saveAfterEdit();
                cmd.SAVE(command, false);
                SelectionContext::updateValues();
                firstEntity.setWorldChanged();
            }
        }
    );
    drawComponent<ScriptListComponent>(
        "Scripts",
        entities,
        false,
        [&](std::unordered_set<Entity> entities, WorldCommandManager &cmd, auto &component) {
            if (isMultiSelect) {
                ImGui::Text("Scripts multi edit not supported");
                return;
            }
            bool modified = false;
            ScriptEngine *scriptEngine = GameContext::getScriptEngine();
            for (auto &script : component.scripts) {
                shiftCursorY(8);
                ImGui::Text("%s", script.getName().c_str());
                // TODO: Add opening script .cpp file in editor or displaying .cpp file name
                ImGui::SameLine();
                if (ImGui::Button(getString(ICON_TRASH_O).c_str())) {
                    for (Entity entity : entities) {
                        entity.removeScript(script);
                    }
                    modified = true;
                    break;
                }
                for (auto &field : script.getFields()) {
                    if (drawScriptFieldValue(field)) {
                        if (world && world->isRunning() && scriptEngine &&
                            scriptEngine->isLoaded() && field.instanceId) {
                            ExternalCalls::setFieldValue(
                                field.instanceId, field.fieldId, &field.value
                            );
                        }
                        modified = true;
                    }
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
                firstEntity.setWorldChanged();
            }
        }
    );
    drawComponent<SpriteRendererComponent>(
        "Sprite Renderer",
        entities,
        true,
        [&](std::unordered_set<Entity> entities, WorldCommandManager &cmd, auto &firstComponent) {
            UpdateSpriteRendererCommand update(entities);
            update.saveBeforeEdit();
            bool modified = false;
            // Color
            bool isInconsistentColor = isInconsistentPrimitive<Color, SpriteRendererComponent>(
                entities, [](const SpriteRendererComponent &other) { return other.color; }
            );
            if (propertyColor("Color", firstComponent.color, isInconsistentColor)) {
                for (Entity entity : entities) {
                    auto &component = entity.getComponent<SpriteRendererComponent>();
                    component.color = firstComponent.color;
                }
                modified = true;
            }
            // Material
            bool isInconsistentMaterial =
                isInconsistentPrimitive<AssetRef<Asset>, SpriteRendererComponent>(
                    entities, [](const SpriteRendererComponent &other) { return other.material; }
                );
            if (propertyMaterial("Material", firstComponent.material, isInconsistentMaterial)) {
                for (Entity entity : entities) {
                    auto &component = entity.getComponent<SpriteRendererComponent>();
                    component.material = firstComponent.material;
                }
                modified = true;
            }
            // Horizontal Images Count
            bool colsInconsistent = isInconsistentPrimitive<int, SpriteRendererComponent>(
                entities, [](const SpriteRendererComponent &other) { return other.cols; }
            );
            if (dragInt("Columns", &firstComponent.cols, 1, 1, 30, "%d", 0, colsInconsistent)) {
                for (Entity entity : entities) {
                    auto &component = entity.getComponent<SpriteRendererComponent>();
                    component.cols = firstComponent.cols;
                }
                modified = true;
            }
            // Vertical Images Count
            bool rowsInconsistent = isInconsistentPrimitive<int, SpriteRendererComponent>(
                entities, [](const SpriteRendererComponent &other) { return other.rows; }
            );
            if (dragInt("Rows", &firstComponent.rows, 1, 1, 30, "%d", 0, rowsInconsistent)) {
                for (Entity entity : entities) {
                    auto &component = entity.getComponent<SpriteRendererComponent>();
                    component.rows = firstComponent.rows;
                }
                modified = true;
            }
            // Current Image Index
            bool indexInconsistent = isInconsistentPrimitive<int, SpriteRendererComponent>(
                entities, [](const SpriteRendererComponent &other) { return other.index; }
            );
            if (dragInt(
                    "Current Index", &firstComponent.index, 1, 0, 900, "%d", 0, indexInconsistent
                )) {
                for (Entity entity : entities) {
                    auto &component = entity.getComponent<SpriteRendererComponent>();
                    component.index = firstComponent.index;
                }
                modified = true;
            }
            if (modified) {
                update.saveAfterEdit();
                cmd.SAVE(update, false);
                firstEntity.setWorldChanged();
            }
        }
    );
    drawComponent<MeshComponent>(
        "Mesh",
        entities,
        false,
        [&](std::unordered_set<Entity> entities, WorldCommandManager &cmd, auto &component) {
            if (isMultiSelect) {
                ImGui::Text("Multi edit not supported");
                return;
            }
            ImGui::Text("Meshes count: %d", (int)component.meshes.size());
        }
    );
    drawComponent<CameraComponent>(
        "Camera",
        entities,
        true,
        [&](std::unordered_set<Entity> entities, WorldCommandManager &cmd, auto &component) {
            if (isMultiSelect) {
                ImGui::Text("Multi edit not supported");
                return;
            }
            ImGuiStyle &style = ImGui::GetStyle();
            auto &camera = component.camera;
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);
            checkbox("Primary", &component.isPrimary, false);
            int selectedPos = combo(
                "Projection",
                {"Perspective", "Orthographic"},
                (int)camera.getProjectionType(),
                false
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
                if (dragFloat("Size", &orthoSize)) { camera.setOrthoSize(orthoSize); }
            }
            float near = camera.getNear();
            if (dragFloat("Near", &near)) { camera.setNear(near); }
            float far = camera.getFar();
            if (dragFloat("Far", &far)) { camera.setFar(far); }
            ImGui::PopStyleVar();
        }
    );
    drawComponent<Rigidbody2DComponent>(
        "Rigidbody 2D",
        entities,
        true,
        [&](std::unordered_set<Entity> entities, WorldCommandManager &cmd, auto &firstComponent) {
            bool modified = false;
            UpdateRigidbody2DCommand update(entities);
            update.saveBeforeEdit();
            // Body type
            const char *bodyTypeStrings[] = {"Static", "Dynamic", "Kinematic"};
            const char *currentBodyTypeString = bodyTypeStrings[(int)firstComponent.type];
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0);
            bool bodyTypeInconsistent = isInconsistentPrimitive<int, Rigidbody2DComponent>(
                entities, [](const Rigidbody2DComponent &other) { return (int)other.type; }
            );
            int selectedPos = combo(
                "Body Type",
                {"Static", "Dynamic", "Kinematic"},
                (int)firstComponent.type,
                bodyTypeInconsistent
            );
            if (selectedPos != (int)firstComponent.type) {
                for (Entity entity : entities) {
                    auto &component = entity.getComponent<Rigidbody2DComponent>();
                    component.type = (Rigidbody2DComponent::BodyType)selectedPos;
                }
                modified = true;
            }
            // Fixed Rotation
            bool fixedRotationInconsistent = isInconsistentPrimitive<bool, Rigidbody2DComponent>(
                entities, [](const Rigidbody2DComponent &other) { return other.fixedRotation; }
            );
            if (checkbox(
                    "Fixed Rotation", &firstComponent.fixedRotation, fixedRotationInconsistent
                )) {
                for (Entity entity : entities) {
                    auto &component = entity.getComponent<Rigidbody2DComponent>();
                    component.fixedRotation = firstComponent.fixedRotation;
                }
                modified = true;
            }
            ImGui::PopStyleVar();
            if (modified) {
                update.saveAfterEdit();
                cmd.SAVE(update);
                firstEntity.setWorldChanged();
            }
        }
    );
    drawComponent<BoxCollider2DComponent>(
        "Box Collider 2D",
        entities,
        true,
        [&](std::unordered_set<Entity> entities, WorldCommandManager &cmd, auto &firstComponent) {
            bool modified = false;
            UpdateBoxCollider2DCommand update(entities);
            update.saveBeforeEdit();
            // Is Sensor
            bool isSensorInconsistent = isInconsistentPrimitive<bool, BoxCollider2DComponent>(
                entities, [](const BoxCollider2DComponent &other) { return other.isSensor; }
            );
            if (checkbox("Is Sensor", &firstComponent.isSensor, isSensorInconsistent)) {
                for (Entity entity : entities) {
                    auto &component = entity.getComponent<BoxCollider2DComponent>();
                    component.isSensor = firstComponent.isSensor;
                }
                modified = true;
            }
            // Offset
            bool offsetInconsistent = isInconsistentPrimitive<glm::vec2, BoxCollider2DComponent>(
                entities, [](const BoxCollider2DComponent &other) { return other.offset; }
            );
            if (dragFloat2(
                    "Offset",
                    glm::value_ptr(firstComponent.offset),
                    0.01f,
                    0.0f,
                    0.0f,
                    "%.3f",
                    0,
                    offsetInconsistent
                )) {
                for (Entity entity : entities) {
                    auto &component = entity.getComponent<BoxCollider2DComponent>();
                    component.offset = firstComponent.offset;
                }
                modified = true;
            }
            // Size
            bool sizeInconsistent = isInconsistentPrimitive<glm::vec2, BoxCollider2DComponent>(
                entities, [](const BoxCollider2DComponent &other) { return other.size; }
            );
            if (dragFloat2(
                    "Size",
                    glm::value_ptr(firstComponent.size),
                    0.01f,
                    0.0f,
                    0.0f,
                    "%.3f",
                    0,
                    sizeInconsistent
                )) {
                for (Entity entity : entities) {
                    auto &component = entity.getComponent<BoxCollider2DComponent>();
                    component.size = firstComponent.size;
                }
                modified = true;
            }
            // Density
            bool densityInconsistent = isInconsistentPrimitive<float, BoxCollider2DComponent>(
                entities, [](const BoxCollider2DComponent &other) { return other.density; }
            );
            if (dragFloat(
                    "Density",
                    &firstComponent.density,
                    0.01f,
                    0.0f,
                    1.0f,
                    "%.3f",
                    0,
                    densityInconsistent
                )) {
                for (Entity entity : entities) {
                    auto &component = entity.getComponent<BoxCollider2DComponent>();
                    component.density = firstComponent.density;
                }
                modified = true;
            }
            // Friction
            bool frictionInconsistent = isInconsistentPrimitive<float, BoxCollider2DComponent>(
                entities, [](const BoxCollider2DComponent &other) { return other.friction; }
            );
            if (dragFloat(
                    "Friction",
                    &firstComponent.friction,
                    0.01f,
                    0.0f,
                    1.0f,
                    "%.3f",
                    0,
                    frictionInconsistent
                )) {
                for (Entity entity : entities) {
                    auto &component = entity.getComponent<BoxCollider2DComponent>();
                    component.friction = firstComponent.friction;
                }
                modified = true;
            }
            // Restitution
            bool restitutionInconsistent = isInconsistentPrimitive<float, BoxCollider2DComponent>(
                entities, [](const BoxCollider2DComponent &other) { return other.restitution; }
            );
            if (dragFloat(
                    "Restitution",
                    &firstComponent.restitution,
                    0.01f,
                    0.0f,
                    1.0f,
                    "%.3f",
                    0,
                    restitutionInconsistent
                )) {
                for (Entity entity : entities) {
                    auto &component = entity.getComponent<BoxCollider2DComponent>();
                    component.restitution = firstComponent.restitution;
                }
                modified = true;
            }
            if (modified) {
                update.saveAfterEdit();
                cmd.SAVE(update);
                firstEntity.setWorldChanged();
            }
        }
    );
    drawComponent<SkyComponent>(
        "Cube Map Rendering",
        entities,
        true,
        [&](std::unordered_set<Entity> entities, WorldCommandManager &cmd, auto &component) {}
    );
}

} // namespace Panda
