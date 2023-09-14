//
// Created by Admin on 25.01.2022.
//

#include "Panda/GameLogic/World.hpp"
#include "Panda/GameLogic/NativeScript.hpp"

namespace Panda {

World::World()
    : m_uiRoot()
    , m_isRunning(true)
    , m_registry() {}

World::~World() {}

void World::update(double deltaTime) {
    if (!m_isRunning) {
        return;
    }
    m_uiRoot.render();
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
    entity.addComponent<TagComponent>();
    entity.addComponent<RelationshipComponent>();
    entity.addComponent<Transform>();
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

UIView *World::getUIView() {
    return &m_uiRoot;
}

} // namespace Panda