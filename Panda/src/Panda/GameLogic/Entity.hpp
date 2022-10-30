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
    void addComponent(Foundation::Shared<Component> component);
    void removeComponent(Foundation::Shared<Component> component);
    template<typename T>
    Foundation::Shared<T> getComponentWithType();
    void addChildEntity(Foundation::Shared<Entity> &entity);
    void removeEntity(Foundation::Shared<Entity> &entity);
    Foundation::Shared<Transform> getTransform() const;
    Entity *getParent();
    std::vector<Foundation::Shared<Entity>> &getChildEntities();

private:
    std::vector<Foundation::Shared<Component>> components;
    std::vector<Foundation::Shared<Entity>> childEntities;
    Entity *parentEntity;
    const Foundation::Shared<Transform> transform;
};

} // namespace Panda