//
// Created by Admin on 09.02.2022.
//

#pragma once

#include "Panda/Window/Window.hpp"
#include "Components/Transform.hpp"
#include "Components/BaseComponents.hpp"

#include <entt/entt.hpp>

namespace Panda {

class Entity final {
public:
    template<typename T>
    T &addNativeScript() {
        NativeScriptListComponent &component = getComponent<NativeScriptListComponent>();
        return component.add<T>();
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

    Transform &getTransform();

    Entity getParent();

    inline const std::vector<id_t> &getChildEntities() {
        return getComponent<RelationshipComponent>().children;
    }

    inline id_t getId() {
        return m_id;
    }

    bool isValid() {
        return m_id != -1;
    }

    void setName(std::string &name) {
        getComponent<TagComponent>().tag = name;
    }

    void setName(std::string name) {
        getComponent<TagComponent>().tag = name;
    }

    friend bool operator==(const Entity &lhs, const Entity &rhs) {
        return lhs.m_id == rhs.m_id;
    }

private:
    Entity(entt::registry *m_registry, id_t id);
    Entity();

    template<typename T, typename... Args>
    T &addComponent(Args &&...args) {
        PND_ASSERT(!hasComponent<T>(), "Entity already has component!");
        entt::entity entity = static_cast<entt::entity>(m_id);
        T &component = m_registry->emplace<T>(entity, std::forward<Args>(args)...);
        return component;
    }

    entt::registry *m_registry;
    id_t m_id;

    friend class World;
    friend class NativeScript;
    friend class WorldHierarchyPanel;
};

} // namespace Panda