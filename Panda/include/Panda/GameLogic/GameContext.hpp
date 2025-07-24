#pragma once

#include "Panda/Assets/Base/AssetHandler.hpp"
#include "Panda/ScriptEngine/ScriptEngine.hpp"

namespace Panda {

class World;

class GameContext {
public:
    static World* getCurrentWorld();
    static void setCurrentWorld(World* world);
    static AssetHandler* getAssetHandler();
    static void setAssetHandler(AssetHandler* handler);
    static ScriptEngine* getScriptEngine();
    static void setScriptEngine(ScriptEngine* engine);

private:
    static World *s_currentWorld;
    static AssetHandler *s_assetHandler;
    static ScriptEngine *s_scriptEngine;
};

} // namespace Panda