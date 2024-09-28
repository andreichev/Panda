//
// Created by Admin on 25.01.2022.
//

#include "Panda/GameLogic/World.hpp"
#include "Panda/GameLogic/Components/SkyComponent.hpp"

#include <Rain/Rain.hpp>
#include <entt/entt.hpp>

namespace Panda {

World::World()
    : m_isRunning(false)
    , m_isChanged(false)
    , m_registry()
    , m_commandManager() {}

World::~World() {}

void World::startRunning() {
    // Update native scripts
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
    m_isRunning = false;
}

void World::updateRuntime(double deltaTime) {
    if (!m_isRunning) {
        return;
    }
    m_renderer2d.begin();
    m_renderer3d.begin();

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

    m_renderer2d.end();
    m_renderer3d.end();
}

void World::updateSimulation(double deltaTime, glm::mat4 &viewProjMtx, glm::mat4 &skyViewProjMtx) {
    m_renderer2d.begin();
    m_renderer3d.begin();

    updateBasicComponents(deltaTime, viewProjMtx, skyViewProjMtx);

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
            id_t entityId = static_cast<id_t>(entityHandle);
            rect.transform = transform.getTransform();
            if (m_selectedEntity.getId() == entityId) {
                rect.color = Color(1.f, 1.f, 0.f, 1.f);
                rect.size = {1.05f, 1.05f};
                m_renderer2d.drawRect(rect);
            }
            rect.color = spriteComponent.color;
            rect.size = {1.f, 1.f};
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
    id_t entityId = static_cast<id_t>(m_registry.create());
    Entity entity = {entityId, this};
    fillEntity(entity);
    m_isChanged = true;
    return entity;
}

Entity World::instantiateEntity(id_t id) {
    m_registry.create(static_cast<entt::entity>(id));
    Entity entity = {id, this};
    fillEntity(entity);
    m_isChanged = true;
    return entity;
}

void World::rebindScriptsAndFields() {
    auto view = m_registry.view<ScriptListComponent>();
    for (auto entityHandle : view) {
        if (!m_registry.valid(entityHandle)) {
            continue;
        }
        auto &component = view.get<ScriptListComponent>(entityHandle);
        id_t entityId = static_cast<id_t>(entityHandle);
        for (auto &container : component.scripts) {
            ScriptHandle scriptId =
                ExternalCalls::addScriptFunc(entityId, container.getName().c_str());
            if (scriptId) {
                container.rebindId(scriptId);
            } else {
                component.remove(container);
                Entity entity = Entity(entityId, this);
                LOG_EDITOR(
                    "SCRIPT {} NOT FOUND. REMOVED FROM ENTITY {}.",
                    container.getName(),
                    entity.getName()
                );
            }
        }
    }
}

void World::fillEntity(Entity entity) {
    entity.addComponent<IdComponent>(entity.m_id);
    entity.addComponent<TagComponent>("Entity");
    entity.addComponent<RelationshipComponent>();
    entity.addComponent<TransformComponent>();
    entity.addComponent<StaticMeshComponent>();
    entity.addComponent<DynamicMeshComponent>();
    entity.addComponent<ScriptListComponent>();
}

void World::destroy(Entity entity) {
    entity.removeFromParent();
    for (id_t childHandle : entity.getChildEntities()) {
        Entity child = Entity(childHandle, this);
        if (!child.isValid()) {
            continue;
        }
        destroy(child);
    }
    m_registry.destroy(static_cast<entt::entity>(entity.getId()));
    m_isChanged = true;
}

void World::clear() {
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
            id_t id = static_cast<id_t>(entity);
            return {id, this};
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
        id_t entityId = static_cast<id_t>(entityHandle);
        if (tagComponent.tag == tag) {
            return Entity(entityId, this);
        }
    }
    return Entity();
}

Entity World::getById(id_t id) {
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
    clear();
    entt::registry &src = other.m_registry;
    entt::registry &dst = m_registry;
    for (auto entity : src.storage<entt::entity>()) {
        auto _ = dst.create(entity);
        copyAllComponents<IdComponent>(src, dst, entity);
        copyAllComponents<TagComponent>(src, dst, entity);
        copyAllComponents<TransformComponent>(src, dst, entity);
        copyAllComponents<RelationshipComponent>(src, dst, entity);
        copyAllComponents<SpriteRendererComponent>(src, dst, entity);
        copyAllComponents<StaticMeshComponent>(src, dst, entity);
        copyAllComponents<DynamicMeshComponent>(src, dst, entity);
        copyAllComponents<CameraComponent>(src, dst, entity);
        copyAllComponents<ScriptListComponent>(src, dst, entity);
        copyAllComponents<SkyComponent>(src, dst, entity);
    }
    return *this;
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