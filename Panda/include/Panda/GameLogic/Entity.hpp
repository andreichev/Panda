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
        return component;
    }

    template<typename T>
    void removeComponent() {
        entt::registry &registry = worldGetRegistry();
        registry.remove<T>(m_handle);
        setWorldChanged();
    }

#pragma region PHYSICS
    /*
    ---------------------------------------------------
                          PHYSICS
    |         |         |         |         |         |
    V         V         V         V         V         V
    */

    template<>
    Rigidbody2DComponent &addComponent<Rigidbody2DComponent>() {
        PND_ASSERT(!hasComponent<Rigidbody2DComponent>(), "Entity already has component!");
        entt::registry &registry = worldGetRegistry();
        Rigidbody2DComponent &component = registry.emplace<Rigidbody2DComponent>(m_handle);
        physics2DRegister();
        setWorldChanged();
        return component;
    }

    template<>
    void removeComponent<Rigidbody2DComponent>() {
        physics2DRemove();
        entt::registry &registry = worldGetRegistry();
        registry.remove<Rigidbody2DComponent>(m_handle);
        setWorldChanged();
    }

    /*
    ^         ^         ^         ^         ^         ^
    |         |         |         |         |         |
                          PHYSICS
    ---------------------------------------------------
    */
#pragma endregion

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

    void physics2DRegister();
    void physics2DUpdate();
    void physics2DRemove();

#ifdef PND_EDITOR
    bool isDeleted();
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