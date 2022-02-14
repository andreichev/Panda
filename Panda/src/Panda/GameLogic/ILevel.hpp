//
// Created by Admin on 25.01.2022.
//

#pragma once

#include "Panda/GameLogic/World.hpp"

namespace Panda {

class ILevel {
public:
    virtual ~ILevel() = default;
    virtual void start(World *world) = 0;
};

} // namespace Panda