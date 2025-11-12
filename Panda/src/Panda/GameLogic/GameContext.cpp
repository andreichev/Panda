#include "Panda/GameLogic/GameContext.hpp"
#include "Panda/GameLogic/SelectionContext.hpp"

namespace Panda {

World *GameContext::s_currentWorld = nullptr;
AssetHandler *GameContext::s_assetHandler = nullptr;
ScriptEngine *GameContext::s_scriptEngine = nullptr;

World *GameContext::getCurrentWorld() {
    return s_currentWorld;
}

void GameContext::setCurrentWorld(World *world) {
    s_currentWorld = world;
    if (!world) {
        SelectionContext::unselectAll();
        return;
    }
    std::unordered_set<UUID> selectedIds = SelectionContext::getSelectedEntities();
    for (UUID id : selectedIds) {
        if (!world->isValid(id)) { SelectionContext::removeSelectedEntity(id, false); }
    }
    SelectionContext::updateValues();
}

AssetHandler *GameContext::getAssetHandler() {
    return s_assetHandler;
}

void GameContext::setAssetHandler(AssetHandler *handler) {
    s_assetHandler = handler;
}

ScriptEngine *GameContext::getScriptEngine() {
    return s_scriptEngine;
}

void GameContext::setScriptEngine(ScriptEngine *engine) {
    s_scriptEngine = engine;
}

} // namespace Panda