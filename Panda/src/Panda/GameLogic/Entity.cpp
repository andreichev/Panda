//
// Created by Admin on 09.02.2022.
//

#include "pndpch.hpp"
#include "Entity.hpp"

namespace Panda {

Entity::Entity()
    : parentEntity(nullptr)
    , transform(makeShared<Transform>()) {
    Transform transform1;
    addComponent(transform);
}

void Entity::addComponent(Shared<Component> component) {
    component->setEntity(this);
    components.push_back(component);
    component->initialize();
}

void Entity::addComponent(Component *component) {
    component->setEntity(this);
    components.push_back(Panda::createShared<Component>(component));
    component->initialize();
}

void Entity::removeComponent(Shared<Component> component) {
    components.erase(find(components.begin(), components.end(), component));
}

template<typename T>
Shared<T> Entity::getComponentWithType() {
    auto &it = find_if(components.begin(), components.end(), [](const auto &item) {
        if (dynamic_cast<T>(item) != nullptr) {
            return true;
        }
        return false;
    });
    if (it == components.end()) {
        return nullptr;
    }
    return *it;
}

void Entity::initialize() {
    for (auto &entity : childEntities) {
        entity->initialize();
    }
    for (auto &component : components) {
        if (component->isActive) {
            component->initialize();
        }
    }
}

void Entity::update(float deltaTime) {
    // Обновить дочерние сущности
    for (auto &entity : childEntities) {
        entity->update(deltaTime);
    }
    // Обновить все компонеты у этой сущности
    for (auto &component : components) {
        if (component->isActive) {
            component->update(deltaTime);
        }
    }
}

Shared<Transform> Entity::getTransform() const {
    return transform;
}

void Entity::addChildEntity(Shared<Entity> &entity) {
    entity->parentEntity = this;
    childEntities.push_back(entity);
}

void Entity::removeEntity(Shared<Entity> &entity) {
    childEntities.erase(find(childEntities.begin(), childEntities.end(), entity));
}

Entity *Entity::getParent() {
    return parentEntity;
}

std::vector<Shared<Entity>> &Entity::getChildEntities() {
    return childEntities;
}

} // namespace Panda
