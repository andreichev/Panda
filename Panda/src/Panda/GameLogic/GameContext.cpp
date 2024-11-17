#include "Panda/GameLogic/GameContext.hpp"

namespace Panda {

World *GameContext::s_currentWorld = nullptr;
AssetHandler *GameContext::s_assetHandler = nullptr;

} // namespace Panda