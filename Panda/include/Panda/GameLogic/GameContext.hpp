#pragma once

#include "World.hpp"

namespace Panda {

class GameContext {
public:
    static World *s_currentWorld;
};

} // namespace Panda