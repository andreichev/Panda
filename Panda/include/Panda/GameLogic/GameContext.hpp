#pragma once

#include "Panda/GameLogic/World.hpp"
#include "Panda/ScriptEngine/ScriptEngine.hpp"

namespace Panda {

class GameContext {
public:
    static World *s_currentWorld;
    static AssetHandler *s_assetHandler;
    static ScriptEngine *s_scriptEngine;
};

} // namespace Panda