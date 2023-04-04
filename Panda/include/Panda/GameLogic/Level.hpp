//
// Created by Admin on 25.01.2022.
//

#pragma once

#include "Panda/GameLogic/World.hpp"

namespace Panda {

class Level {
public:
    virtual ~Level() = default;
    virtual void start(World *world) = 0;
};

} // namespace Panda