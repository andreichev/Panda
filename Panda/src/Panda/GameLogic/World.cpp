//
// Created by Admin on 25.01.2022.
//

#include "Panda/GameLogic/World.hpp"
#include "Panda/GameLogic/GameContext.hpp"
#include "Panda/GameLogic/Components/SkyComponent.hpp"
#include "Panda/Physics/Physics2D.hpp"

#include <Rain/Rain.hpp>
#include <entt/entt.hpp>

namespace Panda {

World::World()
    : m_entityIdMap(100)
    , m_isRunning(false)
    , m_isChanged(false)
    , m_registry()
    , m_commandManager()
    , m_physics2D() {}

World::~World() {
    m_physics2D.destroy();
}

void World::startRunning() {
    // Init physics
    m_physics2D.init(this);
    // Call start at native scripts
    {
        auto view = m_registry.view<ScriptListComponent>();
        for (auto entityHandle : view) {
            if (!m_registry.valid(entityHandle)) {
                continue;
            }
            auto &component = view.get<ScriptListComponent>(entityHandle);
            for (auto &container : component.scripts) {
                container.invokeStart();
            }
        }
    }
    m_isRunning = true;
}

void World::finishRunning() {
    m_physics2D.destroy();
    m_isRunning = false;
}

void World::updateRuntime(double deltaTime) {
    if (!m_isRunning) {
        return;
    }
    m_renderer2d.begin();
    m_renderer3d.begin();

    m_physics2D.update(this, deltaTime);
    // Update native scripts
    {
        auto view = m_registry.view<ScriptListComponent>();
        for (auto entityHandle : view) {
            if (!m_registry.valid(entityHandle)) {
                continue;
            }
            auto &component = view.get<ScriptListComponent>(entityHandle);
            for (auto &container : component.scripts) {
                if (!m_registry.valid(entityHandle)) {
                    break;
                }
                container.invokeUpdate(deltaTime);
            }
        }
    }
    glm::mat4 viewProjMtx;
    glm::mat4 skyViewProjMtx;
    Entity cameraEntity = findMainCameraEntity();
    if (cameraEntity.isValid()) {
        WorldCamera &camera = cameraEntity.getComponent<CameraComponent>().camera;

        glm::mat4 viewMtx = glm::inverse(cameraEntity.getTransform().getTransform());
        glm::mat4 skyViewMtx = glm::inverse(cameraEntity.getTransform().getSkyTransform());
        glm::mat4 projMtx = camera.getProjection();

        viewProjMtx = projMtx * viewMtx;
        skyViewProjMtx = projMtx * skyViewMtx;
        m_renderer2d.setViewProj(viewProjMtx);
        m_renderer3d.setViewProj(viewProjMtx);
    }
    updateBasicComponents(deltaTime, viewProjMtx, skyViewProjMtx);

    m_renderer2d.end();
    m_renderer3d.end();
}

void World::updateSimulation(double deltaTime, glm::mat4 &viewProjMtx, glm::mat4 &skyViewProjMtx) {
    m_renderer2d.begin();
    m_renderer3d.begin();

    m_physics2D.update(this, deltaTime);

    // Update native scripts
    {
        auto view = m_registry.view<ScriptListComponent>();
        for (auto entityHandle : view) {
            if (!m_registry.valid(entityHandle)) {
                continue;
            }
            auto &component = view.get<ScriptListComponent>(entityHandle);
            for (auto &container : component.scripts) {
                container.invokeUpdate(deltaTime);
            }
        }
    }

    updateBasicComponents(deltaTime, viewProjMtx, skyViewProjMtx);

    m_renderer2d.setViewProj(viewProjMtx);
    m_renderer3d.setViewProj(viewProjMtx);

    m_renderer2d.end();
    m_renderer3d.end();
}

void World::updateEditor(double deltaTime, glm::mat4 &viewProjMtx, glm::mat4 &skyViewProjMtx) {
    m_renderer2d.begin();
    m_renderer3d.begin();

    updateBasicComponents(deltaTime, viewProjMtx, skyViewProjMtx);

    m_renderer2d.setViewProj(viewProjMtx);
    m_renderer3d.setViewProj(viewProjMtx);

    m_renderer2d.end();
    m_renderer3d.end();
}

void World::updateBasicComponents(
    float deltaTime, glm::mat4 &viewProjMtx, glm::mat4 &skyViewProjMtx
) {
    // Draw sky
    {
        auto view = m_registry.view<SkyComponent>();
        for (auto &entityHandle : view) {
            if (!m_registry.valid(entityHandle)) {
                continue;
            }
            auto &sky = view.get<SkyComponent>(entityHandle);
            sky.setViewId(m_renderingViewId);
            sky.update(skyViewProjMtx);
        }
    }
    // Render Sprites
    {
        auto view = m_registry.view<SpriteRendererComponent, TransformComponent>();
        for (auto entityHandle : view) {
            if (!m_registry.valid(entityHandle)) {
                continue;
            }
            auto &spriteComponent = view.get<SpriteRendererComponent>(entityHandle);
            auto &transform = view.get<TransformComponent>(entityHandle);
            Panda::Renderer2D::RectData rect;
            rect.transform = transform.getTransform();
            rect.color = spriteComponent.color;
            // Load texture if it needs.
            AssetHandler *assetHandler = GameContext::s_assetHandler;
            if (spriteComponent.textureId && !spriteComponent.texture && assetHandler) {
                spriteComponent.texture = assetHandler->load(spriteComponent.textureId);
            }
            rect.texture = spriteComponent.texture;
            rect.size = {1.f, 1.f};
            rect.id = static_cast<int32_t>(entityHandle);
            m_renderer2d.drawRect(rect);
        }
    }
    // Render static meshes
    {
        auto view = m_registry.view<StaticMeshComponent, TransformComponent>();
        for (auto entityHandle : view) {
            if (!m_registry.valid(entityHandle)) {
                continue;
            }
            auto &staticMeshComponent = view.get<StaticMeshComponent>(entityHandle);
            auto &transform = view.get<TransformComponent>(entityHandle);
            for (auto &mesh : staticMeshComponent.meshes) {
                m_renderer3d.submit(&transform, &mesh);
            }
        }
    }
    // Render dynamic meshes
    {
        auto view = m_registry.view<DynamicMeshComponent, TransformComponent>();
        for (auto entityHandle : view) {
            if (!m_registry.valid(entityHandle)) {
                continue;
            }
            auto &dynamicMeshComponent = view.get<DynamicMeshComponent>(entityHandle);
            auto &transform = view.get<TransformComponent>(entityHandle);
            for (auto &mesh : dynamicMeshComponent.meshes) {
                m_renderer3d.submit(&transform, &mesh);
            }
        }
    }
}

Entity World::instantiateEntity() {
    entt::entity entityHandle = m_registry.create();
    Entity entity = {entityHandle, this};
    fillEntity(entity, {});
    m_entityIdMap[entity.getId()] = entity;
    m_isChanged = true;
    return entity;
}

Entity World::instantiateEntity(UUID id) {
    entt::entity entityHandle = m_registry.create();
    Entity entity = {entityHandle, this};
    fillEntity(entity, id);
    m_entityIdMap[entity.getId()] = entity;
    m_isChanged = true;
    return entity;
}

void World::bindScriptsAndFields() {
    if (!GameContext::s_scriptEngine || !GameContext::s_scriptEngine->isLoaded()) {
        return;
    }
    auto view = m_registry.view<ScriptListComponent>();
    auto manifest = GameContext::s_scriptEngine->getManifest();
    for (auto entityHandle : view) {
        if (!m_registry.valid(entityHandle)) {
            continue;
        }
        auto &component = view.get<ScriptListComponent>(entityHandle);
        UUID entityId = Entity(entityHandle, this).getId();
        //-----------------------------------
        //              CLASSES
        //-----------------------------------
        for (auto &container : component.scripts) {
            ScriptInstanceHandle scriptInstanceId =
                ExternalCalls::instantiateScript(entityId, container.getName().c_str());
            if (!scriptInstanceId) {
                Entity entity = Entity(entityHandle, this);
                LOG_INFO_EDITOR("SCRIPT {} NOT FOUND.", container.getName(), entity.getName());
                // TODO: Remove unbound script after N times unfixed.
                // if(container.unusedCount() > N) {
                //     component.remove(container);
                // }
                continue;
            }
            const ScriptClassManifest &clazz = manifest.getClass(container.getName().c_str());
            if (!clazz) {
                Entity entity = Entity(entityHandle, this);
                LOG_INFO_EDITOR(
                    "SCRIPT CLASS MANIFEST {} NOT FOUND.", container.getName(), entity.getName()
                );
                continue;
            }
            container.rebindId(scriptInstanceId);
            //-----------------------------------
            //              FIELDS
            //-----------------------------------
            for (ScriptField &field : container.getFields()) {
                const ScriptFieldManifest &fieldManifest = clazz.getField(field.name.c_str());
                if (!fieldManifest) {
                    LOG_INFO_EDITOR("SCRIPT {} FIELD {} NOT FOUND.", container.getName(), field.name);
                    container.removeField(field);
                    continue;
                }
            }
            for (auto &fieldManifest : clazz.fields) {
                // Check if field is new - allocate memory and save it
                if (!container.hasField(fieldManifest.name)) {
                    Foundation::Memory value;
                    switch (fieldManifest.type) {
                        case ScriptFieldType::INTEGER: {
                            value = Foundation::Memory::alloc(sizeof(int));
                            memset(value.data, 0, sizeof(int));
                            break;
                        }
                        case ScriptFieldType::ENTITY: {
                            value = Foundation::Memory::alloc(sizeof(UUID));
                            memset(value.data, 0, sizeof(UUID));
                            break;
                        }
                        default: {
                            PND_ASSERT(false, "Unknown field type");
                            break;
                        }
                    }
                    ScriptField field = ScriptField(
                        scriptInstanceId,
                        fieldManifest.handle,
                        fieldManifest.name,
                        fieldManifest.type,
                        value
                    );
                    container.addField(field);
                    continue;
                }
                ScriptField &field = container.getField(fieldManifest.name);
                field.instanceId = scriptInstanceId;
                ExternalCalls::setFieldValue(scriptInstanceId, field.fieldId, field.value.data);
            }
        }
    }
}

void World::fillEntity(Entity entity, UUID id) {
    entity.addComponent<IdComponent>(id);
    entity.addComponent<TagComponent>("Entity");
    entity.addComponent<RelationshipComponent>();
    entity.addComponent<TransformComponent>();
    entity.addComponent<StaticMeshComponent>();
    entity.addComponent<DynamicMeshComponent>();
    entity.addComponent<ScriptListComponent>();
}

void World::destroy(Entity entity) {
    entity.removeFromParent();
    for (UUID childHandle : entity.getChildEntities()) {
        Entity child = getById(childHandle);
        if (!child.isValid()) {
            continue;
        }
        destroy(child);
    }
    m_registry.destroy(entity.m_handle);
    m_isChanged = true;
}

void World::clear() {
    m_isChanged = false;
    m_selectedEntity = Entity();
    for (auto id : m_registry.storage<entt::entity>()) {
        m_registry.destroy(id);
    }
    m_registry.clear();
    m_commandManager.CLEAR();
}

bool World::isEmpty() {
    return !m_registry.storage<entt::entity>().size();
}

Entity World::findMainCameraEntity() {
    auto view = m_registry.view<CameraComponent>();
    for (auto entity : view) {
        if (!m_registry.valid(entity)) {
            continue;
        }
        auto &comp = view.get<CameraComponent>(entity);
        if (comp.isPrimary) {
            return {entity, this};
        }
    }
    return {};
}

Camera *World::findMainCamera() {
    Entity entity = findMainCameraEntity();
    if (!entity.isValid()) {
        return nullptr;
    }
    return &entity.getComponent<CameraComponent>().camera;
}

void World::fillStartupData() {
    Entity cameraEntity = instantiateEntity();
    cameraEntity.setName("Camera");
    cameraEntity.getTransform().setPosition({0.f, 0.f, 4.f});
    cameraEntity.addComponent<CameraComponent>();

    Entity sprite1Entity = instantiateEntity();
    sprite1Entity.setName("Orange Sprite");
    auto &sprite1 = sprite1Entity.addComponent<SpriteRendererComponent>();
    sprite1.color = {1.0f, 0.5f, 0.2f, 1.0f};

    Entity skyEntity = instantiateEntity();
    skyEntity.setName("Sky");
    skyEntity.addComponent<SkyComponent>();
    m_isChanged = true;
}

bool World::isChanged() {
    return m_isChanged;
}

void World::setChanged(bool changed) {
    m_isChanged = changed;
}

Entity World::findByTag(const char *tag) {
    auto view = m_registry.view<TagComponent>();
    for (auto entityHandle : view) {
        if (!m_registry.valid(entityHandle)) {
            continue;
        }
        auto &tagComponent = view.get<TagComponent>(entityHandle);
        if (tagComponent.tag == tag) {
            return Entity(entityHandle, this);
        }
    }
    return Entity();
}

Entity World::getById(UUID id) {
    PND_ASSERT(m_entityIdMap.find(id) != m_entityIdMap.end(), "ENTITY DOES NOT EXISTS");
    return m_entityIdMap.at(id);
}

Entity World::getByEnttId(entt::entity id) {
    PND_ASSERT(m_registry.valid(static_cast<entt::entity>(id)), "ENTITY DOES NOT EXISTS");
    return Entity(id, this);
}

void World::setViewId(Miren::ViewId id) {
    m_renderer2d.setViewId(id);
    m_renderer3d.setViewId(id);
    m_renderingViewId = id;
}

template<typename T>
void copyAllComponents(entt::registry &src, entt::registry &dst, entt::entity entity) {
    if (src.any_of<T>(entity)) {
        dst.emplace<T>(entity, src.get<T>(entity));
    }
}

World &World::operator=(World &other) {
    m_entityIdMap.clear();
    clear();
    entt::registry &src = other.m_registry;
    entt::registry &dst = m_registry;
    for (auto entityHandle : src.storage<entt::entity>()) {
        auto _ = dst.create(entityHandle);
        UUID id = src.get<IdComponent>(entityHandle).id;
        m_entityIdMap[id] = Entity(entityHandle, this);
        copyAllComponents<IdComponent>(src, dst, entityHandle);
        copyAllComponents<TagComponent>(src, dst, entityHandle);
        copyAllComponents<TransformComponent>(src, dst, entityHandle);
        copyAllComponents<RelationshipComponent>(src, dst, entityHandle);
        copyAllComponents<SpriteRendererComponent>(src, dst, entityHandle);
        copyAllComponents<StaticMeshComponent>(src, dst, entityHandle);
        copyAllComponents<DynamicMeshComponent>(src, dst, entityHandle);
        copyAllComponents<CameraComponent>(src, dst, entityHandle);
        copyAllComponents<ScriptListComponent>(src, dst, entityHandle);
        copyAllComponents<SkyComponent>(src, dst, entityHandle);
        copyAllComponents<Rigidbody2DComponent>(src, dst, entityHandle);
        copyAllComponents<BoxCollider2DComponent>(src, dst, entityHandle);
    }
    return *this;
}

void World::physics2DRegisterEntity(Entity entity) {
    m_physics2D.registerEntity(entity);
}

void World::physics2DUpdateEntity(Entity entity) {
    m_physics2D.updateEntity(entity);
}

void World::physics2DRemoveEntity(Entity entity) {
    m_physics2D.removeEntity(entity);
}

void World::debugPrint() {
    LOG_INFO("WORLD DEBUG PRINT");
    {
        auto view = m_registry.view<TagComponent>();
        for (auto entityHandle : view) {
            if (!m_registry.valid(entityHandle)) {
                continue;
            }
            auto &tagComponent = view.get<TagComponent>(entityHandle);
            LOG_INFO("ENTITY: {}", tagComponent.tag);
        }
    }
    LOG_INFO("TOTAL: {} entities", m_registry.storage<entt::entity>().size());
}

} // namespace Panda