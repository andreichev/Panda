//
// Created by Admin on 09.02.2022.
//

#pragma once

#include "Panda/Window/Window.hpp"
#include "Components/Transform.hpp"

namespace Panda {

class Entity {
public:
    Entity();
    void initialize();
    void update(double deltaTime);
    void addComponent(Shared<Component> component);
    void addComponent(Component *component);
    void removeComponent(Shared<Component> component);
    template<typename T>
    Shared<T> getComponentWithType();
    void addChildEntity(Shared<Entity> &entity);
    void removeEntity(Shared<Entity> &entity);
    Shared<Transform> getTransform() const;
    Entity *getParent();
    std::vector<Shared<Entity>> &getChildEntities();

private:
    std::vector<Shared<Component>> components;
    std::vector<Shared<Entity>> childEntities;
    Entity *parentEntity;
    const Shared<Transform> transform;
};

} // namespace Panda