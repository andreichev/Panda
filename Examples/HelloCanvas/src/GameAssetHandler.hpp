#pragma once

#include <Panda/Assets/Base/AssetHandler.hpp>

class GameAssetHandler : public Panda::AssetHandler {
public:
    GameAssetHandler()
        : AssetHandler(Foundation::getAllocator()) {}

    Panda::Asset *loadInternal(Panda::AssetId id) override {
        if (m_loadedAssets.find(id) != m_loadedAssets.end()) { return m_loadedAssets.at(id); }
        return nullptr;
    }
};