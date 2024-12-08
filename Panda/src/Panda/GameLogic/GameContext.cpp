#include "Panda/GameLogic/GameContext.hpp"

namespace Panda {

World *GameContext::s_currentWorld = nullptr;
AssetHandler *GameContext::s_assetHandler = nullptr;
ScriptEngine *GameContext::s_scriptEngine = nullptr;

} // namespace Panda