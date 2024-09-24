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
private:
    inline entt::registry &worldGetRegistry();
    inline void setWorldChanged();

public:
    Entity();

    void addScript(ExternalScript script) {
        ScriptListComponent &component = getComponent<ScriptListComponent>();
        setWorldChanged();
        return component.add(script);
    }

    void removeScript(ExternalScript script) {
        ScriptListComponent &component = getComponent<ScriptListComponent>();
        setWorldChanged();
        return component.remove(script);
    }

    template<typename T, typename... Args>
    T &addComponent(Args &&...args) {
        PND_ASSERT(!hasComponent<T>(), "Entity already has component!");
        entt::entity entity = static_cast<entt::entity>(m_id);
        entt::registry &registry = worldGetRegistry();
        T &component = registry.emplace<T>(entity, std::forward<Args>(args)...);
        setWorldChanged();
        return component;
    }

    template<typename T>
    void removeComponent() {
        entt::entity entityHandle = static_cast<entt::entity>(m_id);
        entt::registry &registry = worldGetRegistry();
        registry.remove<T>(entityHandle);
        setWorldChanged();
    }

    template<typename T>
    T &getComponent() {
        PND_ASSERT(hasComponent<T>(), "Entity doesn't have component!");
        entt::entity entityHandle = static_cast<entt::entity>(m_id);
        entt::registry &registry = worldGetRegistry();
        return registry.get<T>(entityHandle);
    }

    template<typename T>
    bool hasComponent() {
        entt::entity entityHandle = static_cast<entt::entity>(m_id);
        entt::registry &registry = worldGetRegistry();
        return registry.any_of<T>(entityHandle);
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
        entt::registry &registry = worldGetRegistry();
        return m_id != -1 && registry.valid(static_cast<entt::entity>(m_id));
    }

    std::string &getName() {
        return getComponent<TagComponent>().tag;
    }

    void setName(const std::string &name) {
        auto& tagComponent = getComponent<TagComponent>();
        if(tagComponent.tag == name) {
            return;
        }
        tagComponent.tag = name;
        setWorldChanged();
    }

    World *getWorld() {
        return m_world;
    }

    friend bool operator==(const Entity &lhs, const Entity &rhs) {
        return lhs.m_id == rhs.m_id;
    }

private:
    Entity(id_t id, World *world);

    World *m_world;
    id_t m_id;

    friend class World;
    friend class WorldHierarchyPanel;
    friend class WorldMapper;
};

} // namespace Panda