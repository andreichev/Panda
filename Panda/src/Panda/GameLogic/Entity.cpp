//
// Created by Admin on 09.02.2022.
//

#include "Panda/GameLogic/Entity.hpp"
#include "Panda/GameLogic/World.hpp"

namespace Panda {

Entity::Entity(id_t id, World *world)
    : m_id(id)
    , m_world(world) {}

Entity::Entity()
    : m_id(-1)
    , m_world(nullptr) {}

entt::registry &Entity::worldGetRegistry() {
    return m_world->m_registry;
}

void Entity::setWorldChanged() {
    m_world->setChanged();
}

TransformComponent &Entity::getTransform() {
    return getComponent<TransformComponent>();
}

void Entity::setTransform(TransformComponent &transform) {
    TransformComponent &_transform = getTransform();
    _transform.setTransform(transform);
}

void Entity::addChildEntity(Entity entity) {
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    RelationshipComponent &otherRelationship = entity.getComponent<RelationshipComponent>();

    otherRelationship.parentHandle = m_id;
    thisRelationship.children.push_back(entity.m_id);
    setWorldChanged();
}

void Entity::removeChildEntity(Entity entity) {
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    RelationshipComponent &otherRelationship = entity.getComponent<RelationshipComponent>();

    std::remove(thisRelationship.children.begin(), thisRelationship.children.end(), entity.m_id);
    otherRelationship.parentHandle = -1;
    setWorldChanged();
}

void Entity::removeFromParent() {
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    if (thisRelationship.parentHandle != -1) {
        Entity parent = Entity(thisRelationship.parentHandle, m_world);
        parent.removeChildEntity(*this);
    }
}

Entity Entity::getParent() {
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    return Entity(thisRelationship.parentHandle, m_world);
}

} // namespace Panda
