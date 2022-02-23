//
// Created by Admin on 25.01.2022.
//

#pragma once

#include "Panda/GameLogic/Entity.hpp"
#include "Panda/Window/Window.hpp"
#include "Panda/Events/Input.hpp"
#include "Panda/Renderer/Renderer.hpp"

namespace Panda {

class World {
public:
    World();
    void initialize();
    void update(double deltaTime);
    Shared<Entity> instantiateEntity();
    void destroy(Shared<Entity> &entity);

private:
    Entity root;
};

} // namespace Panda