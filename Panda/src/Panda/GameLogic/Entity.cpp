//
// Created by Admin on 09.02.2022.
//

#include "Panda/GameLogic/Entity.hpp"
#include "Panda/GameLogic/World.hpp"

namespace Panda {

Entity::Entity(entt::entity handle, World *world)
    : m_handle(handle)
    , m_world(world) {}

Entity::Entity()
    : m_handle(entt::null)
    , m_world(nullptr) {}

entt::registry &Entity::worldGetRegistry() {
    return m_world->m_registry;
}

void Entity::setWorldChanged(bool changed) {
    m_world->setChanged(changed);
}

TransformComponent &Entity::getTransform() {
    return getComponent<TransformComponent>();
}

void Entity::addChildEntity(Entity entity) {
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    RelationshipComponent &otherRelationship = entity.getComponent<RelationshipComponent>();

    otherRelationship.parent = getId();
    thisRelationship.children.push_back(entity.getId());
    setWorldChanged();
}

void Entity::removeChildEntity(Entity entity) {
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    RelationshipComponent &otherRelationship = entity.getComponent<RelationshipComponent>();

    auto _ = std::remove(
        thisRelationship.children.begin(), thisRelationship.children.end(), entity.getId()
    );
    otherRelationship.parent = 0;
    setWorldChanged();
}

void Entity::removeFromParent() {
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    if (thisRelationship.parent != 0) {
        Entity parent = m_world->getById(thisRelationship.parent);
        parent.removeChildEntity(*this);
    }
}

Entity Entity::getParent() {
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    return m_world->getById(thisRelationship.parent);
}

bool Entity::isValid() const {
    return (m_handle != entt::null) && m_world && m_world->m_registry.valid(m_handle);
}

Entity::operator bool() const {
    return isValid();
}

void Entity::physics2DRegister() {
    m_world->physics2DRegisterEntity(*this);
}

void Entity::physics2DUpdate() {
    m_world->physics2DUpdateEntity(*this);
}

void Entity::physics2DRemove() {
    m_world->physics2DRemoveEntity(*this);
}

} // namespace Panda
