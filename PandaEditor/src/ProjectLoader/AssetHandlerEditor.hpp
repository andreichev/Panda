#pragma once

#include "Serialization/AssetRegistryMapper.hpp"

#include <Panda/Base/FileBase.hpp>
#include <Rain/Coders/JsonDecoder.hpp>
#include <Rain/Coders/JsonEncoder.hpp>
#include <Panda/Assets/AssetHandler.hpp>

namespace Panda {

class AssetHandlerEditor : public AssetHandler {
public:
    AssetHandlerEditor();
    Foundation::Shared<Asset> load(AssetId id) override;
    void openProject(const path_t path);
    void closeProject();

private:
    void loadAssetRegistry();
    void saveAssetRegistry();
    void unloadAllAssets();

    path_t m_projectPath;
    path_t m_assetRegistryPath;
    Rain::JsonEncoder m_jsonEncoder;
    Rain::JsonDecoder m_jsonDecoder;
    std::unordered_map<AssetId, Foundation::Shared<Asset>> m_cache;
    std::unordered_map<AssetId, AssetInfoEditor> m_registry;
};

} // namespace Panda