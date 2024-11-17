#pragma once

#include "World.hpp"

namespace Panda {

class GameContext {
public:
    static World *s_currentWorld;
    static AssetHandler *s_assetHandler;
};

} // namespace Panda