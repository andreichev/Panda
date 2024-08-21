//
// Created by Admin on 09.02.2022.
//

#pragma once

#include "Panda/Window/Window.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/BaseComponents.hpp"

#include <entt/entt.hpp>

namespace Panda {

class World;

class Entity final {
public:
    Entity();

    void addScript(ExternalScript script) {
        ScriptListComponent &component = getComponent<ScriptListComponent>();
        return component.add(script);
    }

    template<typename T, typename... Args>
    T &addComponent(Args &&...args) {
        PND_ASSERT(!hasComponent<T>(), "Entity already has component!");
        entt::entity entity = static_cast<entt::entity>(m_id);
        T &component = m_registry->emplace<T>(entity, std::forward<Args>(args)...);
        return component;
    }

    template<typename T>
    void removeComponent() {
        entt::entity entityHandle = static_cast<entt::entity>(m_id);
        m_registry->remove<T>(entityHandle);
    }

    template<typename T>
    T &getComponent() {
        PND_ASSERT(hasComponent<T>(), "Entity doesn't have component!");
        entt::registry &registry = *reinterpret_cast<entt::registry *>(m_registry);
        entt::entity entityHandle = static_cast<entt::entity>(m_id);
        return registry.get<T>(entityHandle);
    }

    template<typename T>
    bool hasComponent() {
        entt::entity entityHandle = static_cast<entt::entity>(m_id);
        return m_registry->any_of<T>(entityHandle);
    }

    void addChildEntity(Entity entity);

    void removeChildEntity(Entity entity);

    void removeFromParent();

    TransformComponent &getTransform();

    Entity getParent();

    const std::vector<id_t> &getChildEntities() {
        return getComponent<RelationshipComponent>().children;
    }

    id_t getId() {
        return m_id;
    }

    bool isValid() {
        return m_id != -1 && m_registry->valid(static_cast<entt::entity>(m_id));
    }

    std::string &getName() {
        return getComponent<TagComponent>().tag;
    }

    void setName(const std::string &name) {
        getComponent<TagComponent>().tag = name;
    }

    World *getWorld() {
        return m_world;
    }

    friend bool operator==(const Entity &lhs, const Entity &rhs) {
        return lhs.m_id == rhs.m_id;
    }

private:
    Entity(entt::registry *m_registry, id_t id, World *world);

    entt::registry *m_registry;
    World *m_world;
    id_t m_id;

    friend class World;
    friend class WorldHierarchyPanel;
    friend class WorldMapper;
};

} // namespace Panda