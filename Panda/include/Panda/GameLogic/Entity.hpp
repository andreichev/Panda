//
// Created by Admin on 09.02.2022.
//

#pragma once

#include "Components/TransformComponent.hpp"
#include "Components/BaseComponents.hpp"

#include <entt/entt.hpp>

namespace Panda {

class World;

class Entity final {
private:
    entt::registry &worldGetRegistry() const;

public:
    Entity();

    void addScript(ExternalScript script) {
        ScriptListComponent &component = getComponent<ScriptListComponent>();
#ifdef PND_EDITOR
        setWorldChanged();
#endif
        return component.add(script);
    }

    void removeScript(ExternalScript script) {
        ScriptListComponent &component = getComponent<ScriptListComponent>();
#ifdef PND_EDITOR
        setWorldChanged();
#endif
        return component.remove(script);
    }

    template<typename T, typename... Args>
    T &addComponent(Args &&...args) {
        PND_ASSERT(!hasComponent<T>(), "Entity already has component!");
        entt::registry &registry = worldGetRegistry();
        T &component = registry.emplace<T>(m_handle, std::forward<Args>(args)...);
#ifdef PND_EDITOR
        setWorldChanged();
#endif
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
#ifdef PND_EDITOR
        setWorldChanged();
#endif
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
    T &getComponent() const {
        PND_ASSERT(hasComponent<T>(), "Entity doesn't have component!");
        entt::registry &registry = worldGetRegistry();
        return registry.get<T>(m_handle);
    }

    template<typename T>
    bool hasComponent() const {
        entt::registry &registry = worldGetRegistry();
        return registry.any_of<T>(m_handle);
    }

    void addChildEntity(Entity entity);

    void removeChildEntity(Entity entity);

    void removeFromParent();

    bool hasChild(Entity entity);

    bool hasAnyChild();

    bool isAncestorOf(Entity entity);

    bool isDescendantOf(Entity entity);

    TransformComponent &getTransform();

    TransformComponent calculateWorldSpaceTransform();

    template<typename T>
    void setComponent(T &value) {
        getComponent<T>() = value;
    }

    Entity getParent();

    const std::vector<UUID> &getChildEntities() {
        return getComponent<RelationshipComponent>().children;
    }

    bool isValid() const;

    std::string &getName() const {
        return getComponent<TagComponent>().tag;
    }

    void setName(const std::string &name) {
        auto &tagComponent = getComponent<TagComponent>();
        if (tagComponent.tag == name) { return; }
        tagComponent.tag = name;
#ifdef PND_EDITOR
        setWorldChanged();
#endif
    }

    World *getWorld() {
        return m_world;
    }

    friend bool operator==(const Entity &lhs, const Entity &rhs) {
        return lhs.m_handle == rhs.m_handle;
    }

    operator entt::entity() const {
        return m_handle;
    }
    operator bool() const;

    UUID getId() const {
        return getComponent<IdComponent>().id;
    }

    void physics2DComponentsUpdated();
    void physics2DPropertiesUpdated();

#ifdef PND_EDITOR
    void setWorldChanged(bool changed = true);
    bool needToDestroy() const;
    bool isDeleted() const;
    void sortWorld() const;
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
    friend class AddRemoveEntitiesCommand;
};

} // namespace Panda

namespace std {

using namespace Panda;

template<>
struct hash<Entity> {
    std::size_t operator()(const Panda::Entity &entity) const {
        return entity.getId();
    }
};

} // namespace std