#pragma once

#include <Panda/Assets/Base/AssetHandler.hpp>

class GameAssetHandler final : public Panda::AssetHandler {
public:
    GameAssetHandler()
        : AssetHandler(Foundation::getAllocator()) {}

    Panda::Asset *loadInternal(Panda::AssetId id, bool forcedReload) override {
        if (m_loadedAssets.find(id) != m_loadedAssets.end()) { return m_loadedAssets.at(id).asset; }
        return nullptr;
    }
};