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
    entt::registry &worldGetRegistry();

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
        entt::registry &registry = worldGetRegistry();
        T &component = registry.emplace<T>(m_handle, std::forward<Args>(args)...);
        setWorldChanged();
        if constexpr (std::is_same_v<T, Rigidbody2DComponent> ||
                      std::is_same_v<T, BoxCollider2DComponent>) {
            physics2DComponentsUpdated();
        }
        return component;
    }

    template<typename T>
    void removeComponent() {
        entt::registry &registry = worldGetRegistry();
        registry.remove<T>(m_handle);
        setWorldChanged();
        if constexpr (std::is_same_v<T, Rigidbody2DComponent> ||
                      std::is_same_v<T, BoxCollider2DComponent>) {
            physics2DComponentsUpdated();
        }
    }

    template<typename T>
    T &getComponent() {
        PND_ASSERT(hasComponent<T>(), "Entity doesn't have component!");
        entt::registry &registry = worldGetRegistry();
        return registry.get<T>(m_handle);
    }

    template<typename T>
    bool hasComponent() {
        entt::registry &registry = worldGetRegistry();
        return registry.any_of<T>(m_handle);
    }

    void addChildEntity(Entity entity);

    void removeChildEntity(Entity entity);

    void removeFromParent();

    TransformComponent &getTransform();

    template<typename T>
    void setComponent(T &value) {
        getComponent<T>() = value;
    }

    Entity getParent();

    const std::vector<UUID> &getChildEntities() {
        return getComponent<RelationshipComponent>().children;
    }

    bool isValid() const;

    std::string &getName() {
        return getComponent<TagComponent>().tag;
    }

    void setName(const std::string &name) {
        auto &tagComponent = getComponent<TagComponent>();
        if (tagComponent.tag == name) {
            return;
        }
        tagComponent.tag = name;
        setWorldChanged();
    }

    World *getWorld() {
        return m_world;
    }

    friend bool operator==(const Entity &lhs, const Entity &rhs) {
        return lhs.m_handle == rhs.m_handle;
    }

    void setWorldChanged(bool changed = true);

    operator entt::entity() const {
        return m_handle;
    }
    operator bool() const;

    UUID getId() {
        return getComponent<IdComponent>().id;
    }

    void physics2DComponentsUpdated();
    void physics2DPropertiesUpdated();

#ifdef PND_EDITOR
    bool isDeleted();
    void sortWorld();
    void setDeleted(bool deleted = true);
#endif

private:
    Entity(entt::entity handle, World *world);

    World *m_world;
    entt::entity m_handle;

    friend class World;
    friend class Physics2D;
    friend class WorldHierarchyPanel;
    friend class WorldMapper;
    friend class AddRemoveEntityCommand;
};

} // namespace Panda