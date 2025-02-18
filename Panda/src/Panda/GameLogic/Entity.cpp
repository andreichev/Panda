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

entt::registry &Entity::worldGetRegistry() const {
    return m_world->m_registry;
}

void Entity::setWorldChanged(bool changed) {
    m_world->setChanged(changed);
}

TransformComponent &Entity::getTransform() {
    return getComponent<TransformComponent>();
}

TransformComponent Entity::calculateWorldSpaceTransform() {
    TransformComponent transformComponent;
    glm::mat4 transform = m_world->getWorldSpaceTransformMatrix(*this);
    transformComponent.setTransform(transform);
    return transformComponent;
}

void Entity::addChildEntity(Entity entity) {
    if (entity.isAncestorOf(*this)) {
        LOG_ERROR_EDITOR(
            "The entity %s can't be moved into one of its children.", entity.getName().c_str()
        );
        return;
    }
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    RelationshipComponent &otherRelationship = entity.getComponent<RelationshipComponent>();

    entity.removeFromParent();
    otherRelationship.parent = getId();
    thisRelationship.children.push_back(entity.getId());
    m_world->convertToLocalSpace(entity);
    setWorldChanged();
}

void Entity::removeChildEntity(Entity entity) {
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    RelationshipComponent &otherRelationship = entity.getComponent<RelationshipComponent>();
    auto &children = thisRelationship.children;
    children.erase(std::remove(children.begin(), children.end(), entity.getId()), children.end());
    m_world->convertToWorldSpace(entity);
    otherRelationship.parent = 0;
    setWorldChanged();
}

void Entity::removeFromParent() {
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    if (!thisRelationship.parent) { return; }
    Entity parent = m_world->getById(thisRelationship.parent);
    parent.removeChildEntity(*this);
}

bool Entity::hasChild(Entity entity) {
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    auto &children = thisRelationship.children;
    return std::find(children.begin(), children.end(), entity.getId()) != children.end();
}

bool Entity::hasAnyChild() {
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    auto &children = thisRelationship.children;
#ifdef PND_EDITOR
    std::vector<UUID> filtered;
    std::copy_if(
        children.begin(),
        children.end(),
        std::back_inserter(filtered),
        [this](UUID entityId) {
            entt::registry &registry = m_world->m_registry;
            PND_ASSERT(
                m_world->m_entityIdMap.find(entityId) != m_world->m_entityIdMap.end(),
                "ENTITY DELETED"
            );
            entt::entity handle = m_world->m_entityIdMap.at(entityId);
            auto &metadata = registry.get<EditorMetadataComponent>(handle);
            return !metadata.isDeleted;
        }
    );
    return !filtered.empty();
#else
    return !children.empty();
#endif
}

bool Entity::isAncestorOf(Entity entity) {
    auto &children = getChildEntities();
    if (children.empty()) { return false; }
    for (UUID childId : children) {
        if (childId == entity.getId()) { return true; }
    }
    for (UUID childId : children) {
        if (m_world->getById(childId).isAncestorOf(entity)) { return true; }
    }
    return false;
}

bool Entity::isDescendantOf(Entity entity) {
    return entity.isAncestorOf(*this);
}

Entity Entity::getParent() {
    RelationshipComponent &thisRelationship = getComponent<RelationshipComponent>();
    if (!thisRelationship.parent) { return {}; }
    return m_world->getById(thisRelationship.parent);
}

bool Entity::isValid() const {
    return (m_handle != entt::null) && m_world && m_world->isValidEntt(m_handle);
}

Entity::operator bool() const {
    return isValid();
}

void Entity::physics2DComponentsUpdated() {
    m_world->physics2DComponentsUpdatedAt(*this);
}

void Entity::physics2DPropertiesUpdated() {
    m_world->physics2DPropertiesUpdatedAt(*this);
}

#ifdef PND_EDITOR
bool Entity::needToDestroy() const {
    return m_world->needToDestroy(*this);
}

bool Entity::isDeleted() const {
    return m_world->isDeleted(m_handle);
}

void Entity::sortWorld() const {
    m_world->sort();
}

void Entity::setDeleted(bool deleted) {
    getComponent<EditorMetadataComponent>().isDeleted = deleted;
}
#endif

} // namespace Panda
