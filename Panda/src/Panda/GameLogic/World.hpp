//
// Created by Admin on 25.01.2022.
//

#pragma once

#include "Panda/GameLogic/Entity.hpp"
#include "Panda/Window/IWindow.hpp"
#include "Panda/Events/IEvents.hpp"
#include "Panda/Renderer/IRenderer.hpp"

namespace Panda {

class World {
public:
    World();
    void initialize();
    void update(float deltaTime);
    Shared<Entity> instantiateEntity();
    void destroy(Shared<Entity> &entity);

private:
    Entity root;
};

} // namespace Panda