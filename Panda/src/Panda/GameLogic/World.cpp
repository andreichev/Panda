//
// Created by Admin on 25.01.2022.
//

#include "Panda/GameLogic/World.hpp"
#include "Panda/GameLogic/NativeScript.hpp"
#include "Panda/Renderer/Renderer3D.hpp"

namespace Panda {

World::World()
    : m_isRunning(false)
    , m_registry() {}

World::~World() {}

void World::update(double deltaTime) {
    if (!m_isRunning) {
        return;
    }
    // Update native scripts
    {
        auto view = m_registry.view<NativeScriptListComponent>();
        for (auto entityHandle : view) {
            auto &component = view.get<NativeScriptListComponent>(entityHandle);
            for (auto &container : component.scripts) {
                if (!container.initialized) {
                    id_t entityId = static_cast<id_t>(entityHandle);
                    container.instance->setEntity({&m_registry, entityId});
                    container.instance->initialize();
                    container.initialized = true;
                }
                container.instance->update(deltaTime);
            }
        }
    }
    // Render static meshes
    {
        auto view = m_registry.view<StaticMeshComponent, Transform>();
        for (auto entityHandle : view) {
            auto &staticMeshComponent = view.get<StaticMeshComponent>(entityHandle);
            auto &transform = view.get<Transform>(entityHandle);
            for (auto &mesh : staticMeshComponent.meshes) {
                Renderer3D::submit(&transform, mesh.get());
            }
        }
    }
    // Render dynamic meshes
    {
        auto view = m_registry.view<DynamicMeshComponent, Transform>();
        for (auto entityHandle : view) {
            auto &dynamicMeshComponent = view.get<DynamicMeshComponent>(entityHandle);
            auto &transform = view.get<Transform>(entityHandle);
            for (auto &mesh : dynamicMeshComponent.meshes) {
                Renderer3D::submit(&transform, mesh.get());
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
    Entity entity = {&m_registry, entityId};
    entity.addComponent<IdComponent>();
    entity.addComponent<TagComponent>("Entity");
    entity.addComponent<RelationshipComponent>();
    entity.addComponent<Transform>();
    entity.addComponent<StaticMeshComponent>();
    entity.addComponent<DynamicMeshComponent>();
    entity.addComponent<NativeScriptListComponent>();
    return entity;
}

void World::destroy(Entity entity) {
    m_registry.destroy(static_cast<entt::entity>(entity.getId()));
    entity.removeFromParent();
    for (id_t childHandle : entity.getChildEntities()) {
        Entity child = Entity(&m_registry, childHandle);
        destroy(child);
    }
}

} // namespace Panda