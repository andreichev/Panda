#pragma once

#include "Panda/Assets/Base/AssetHandler.hpp"
#include "Panda/ScriptEngine/ScriptEngine.hpp"

namespace Panda {

class World;

class GameContext {
public:
    static World *s_currentWorld;
    static AssetHandler *s_assetHandler;
    static ScriptEngine *s_scriptEngine;
};

} // namespace Panda