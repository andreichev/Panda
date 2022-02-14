//
// Created by Admin on 25.01.2022.
//

#include "pndpch.hpp"
#include "World.hpp"

namespace Panda {

World::World()
    : root() {}

void World::update(float deltaTime) {
    root.update(deltaTime);
}

void World::initialize() {
    root.initialize();
}

Shared<Entity> World::instantiateEntity() {
    Shared<Entity> entity = createShared<Entity>();
    root.addChildEntity(entity);
    return entity;
}

void World::destroy(Shared<Entity> &entity) {
    root.removeEntity(entity);
}

} // namespace Panda