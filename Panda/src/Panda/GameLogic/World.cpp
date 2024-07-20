//
// Created by Admin on 25.01.2022.
//

#include "Panda/GameLogic/World.hpp"
#include "Panda/GameLogic/NativeScript.hpp"

#include <Rain/Rain.hpp>
#include <entt/entt.hpp>

namespace Panda {

World::World()
    : m_isRunning(false)
    , m_registry() {}

World::~World() {}

void World::updateRuntime(double deltaTime) {
    if (!m_isRunning) {
        return;
    }
    m_renderer2d.begin();
    m_renderer3d.begin();
    // Update native scripts
    {
        auto view = m_registry.view<NativeScriptListComponent>();
        for (auto entityHandle : view) {
            auto &component = view.get<NativeScriptListComponent>(entityHandle);
            for (auto &container : component.scripts) {
                if (!container.initialized) {
                    id_t entityId = static_cast<id_t>(entityHandle);
                    container.instance->setEntity({&m_registry, entityId, this});
                    container.instance->initialize();
                    container.initialized = true;
                }
                container.instance->update(deltaTime);
            }
        }
    }
    updateBasicComponents(deltaTime);

    Entity cameraEntity = findMainCameraEntity();
    if (cameraEntity.isValid()) {
        WorldCamera &camera = cameraEntity.getComponent<CameraComponent>().camera;

        glm::mat4 view = glm::inverse(cameraEntity.getTransform().getTransform());
        glm::mat4 viewProj = camera.getProjection() * view;
        m_renderer2d.setViewProj(viewProj);
        m_renderer3d.setViewProj(viewProj);
    }

    m_renderer2d.end();
    m_renderer3d.end();
}

void World::updateSimulation(double deltaTime, glm::mat4 viewProjectionMatrix) {
    m_renderer2d.begin();
    m_renderer3d.begin();

    // Update native scripts
    {
        auto view = m_registry.view<NativeScriptListComponent>();
        for (auto entityHandle : view) {
            auto &component = view.get<NativeScriptListComponent>(entityHandle);
            for (auto &container : component.scripts) {
                if (!container.initialized) {
                    id_t entityId = static_cast<id_t>(entityHandle);
                    container.instance->setEntity({&m_registry, entityId, this});
                    container.instance->initialize();
                    container.initialized = true;
                }
                container.instance->update(deltaTime);
            }
        }
    }
    updateBasicComponents(deltaTime);

    m_renderer2d.setViewProj(viewProjectionMatrix);
    m_renderer3d.setViewProj(viewProjectionMatrix);

    m_renderer2d.end();
    m_renderer3d.end();
}

void World::updateEditor(double deltaTime, glm::mat4 viewProjectionMatrix) {
    m_renderer2d.begin();
    m_renderer3d.begin();

    updateBasicComponents(deltaTime);

    m_renderer2d.setViewProj(viewProjectionMatrix);
    m_renderer3d.setViewProj(viewProjectionMatrix);

    m_renderer2d.end();
    m_renderer3d.end();
}

void World::updateBasicComponents(float deltaTime) {
    // Render Sprites
    {
        auto view = m_registry.view<SpriteRendererComponent, TransformComponent>();
        for (auto entityHandle : view) {
            auto &spriteComponent = view.get<SpriteRendererComponent>(entityHandle);
            auto &transform = view.get<TransformComponent>(entityHandle);
            Panda::Renderer2D::RectData rect;
            rect.color = spriteComponent.color;
            rect.size = {1.f, 1.f};
            rect.center = transform.getPosition();
            rect.rotation = transform.getRotationEuler().z;
            m_renderer2d.drawRect(rect);
        }
    }
    // Render static meshes
    {
        auto view = m_registry.view<StaticMeshComponent, TransformComponent>();
        for (auto entityHandle : view) {
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
            auto &dynamicMeshComponent = view.get<DynamicMeshComponent>(entityHandle);
            auto &transform = view.get<TransformComponent>(entityHandle);
            for (auto &mesh : dynamicMeshComponent.meshes) {
                m_renderer3d.submit(&transform, &mesh);
            }
        }
    }
}

void World::onImGuiRender() {
    auto view = m_registry.view<NativeScriptListComponent>();
    for (auto entityHandle : view) {
        auto &component = view.get<NativeScriptListComponent>(entityHandle);
        for (auto &container : component.scripts) {
            if (container.initialized) {
                container.instance->onImGuiRender();
            }
        }
    }
}

void World::initialize() {
    m_isRunning = true;
}

Entity World::instantiateEntity() {
    id_t entityId = static_cast<id_t>(m_registry.create());
    Entity entity = {&m_registry, entityId, this};
    entity.addComponent<IdComponent>();
    entity.addComponent<TagComponent>("Entity");
    entity.addComponent<RelationshipComponent>();
    entity.addComponent<TransformComponent>();
    entity.addComponent<StaticMeshComponent>();
    entity.addComponent<DynamicMeshComponent>();
    entity.addComponent<NativeScriptListComponent>();
    return entity;
}

void World::destroy(Entity entity) {
    entity.removeFromParent();
    for (id_t childHandle : entity.getChildEntities()) {
        Entity child = Entity(&m_registry, childHandle, this);
        destroy(child);
    }
    m_registry.destroy(static_cast<entt::entity>(entity.getId()));
}

void World::clear() {
    m_registry.clear();
}

Entity World::findMainCameraEntity() {
    auto view = m_registry.view<CameraComponent>();
    for (auto entity : view) {
        auto &comp = view.get<CameraComponent>(entity);
        if (comp.isPrimary) {
            id_t id = static_cast<id_t>(entity);
            return {&m_registry, id, this};
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

World &World::operator=(const World &other) {
    const entt::registry &source = other.m_registry;
    entt::registry &destination = m_registry;
    destination.storage<entt::entity>().push(
        source.storage<entt::entity>()->begin(), source.storage<entt::entity>()->end()
    );
    return *this;
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
}

bool World::isChanged() {
    return true;
}

} // namespace Panda