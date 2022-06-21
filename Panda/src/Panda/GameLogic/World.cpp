//
// Created by Admin on 25.01.2022.
//

#include "pndpch.hpp"
#include "World.hpp"

namespace Panda {

World::World()
    : root()
    , uiRoot() {}

void World::update(double deltaTime) {
    root.update(deltaTime);
    uiRoot.render();
}

void World::initialize() {
    root.initialize();
}

Shared<Entity> World::instantiateEntity() {
    Shared<Entity> entity = makeShared<Entity>();
    root.addChildEntity(entity);
    return entity;
}

void World::destroy(Shared<Entity> &entity) {
    root.removeEntity(entity);
    for (Shared<Entity> child : entity->getChildEntities()) {
        destroy(child);
    }
}

UIView *World::getUIView() {
    return &uiRoot;
}

} // namespace Panda