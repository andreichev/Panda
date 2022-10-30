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

Foundation::Shared<Entity> World::instantiateEntity() {
    Foundation::Shared<Entity> entity = Foundation::makeShared<Entity>();
    root.addChildEntity(entity);
    return entity;
}

void World::destroy(Foundation::Shared<Entity> &entity) {
    root.removeEntity(entity);
    for (Foundation::Shared<Entity> child : entity->getChildEntities()) {
        destroy(child);
    }
}

UIView *World::getUIView() {
    return &uiRoot;
}

} // namespace Panda