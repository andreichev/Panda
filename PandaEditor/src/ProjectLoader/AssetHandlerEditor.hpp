#pragma once

#include "Serialization/AssetRegistryMapper.hpp"

#include <Panda/Assets/Path.hpp>
#include <Rain/Coders/JsonDecoder.hpp>
#include <Rain/Coders/JsonEncoder.hpp>
#include <Panda/Assets/AssetHandler.hpp>

namespace Panda {

class AssetHandlerEditor : public AssetHandler {
public:
    AssetHandlerEditor();
    ~AssetHandlerEditor();
    Foundation::Shared<Asset> load(AssetId id) override;
    void importAsset(const path_t &path);
    UUID getAssetId(path_t path);
    void openProject(const path_t &path);
    void closeProject();
    static inline AssetHandlerEditor *getInstance() {
        return s_instance;
    }

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
    std::unordered_map<path_t, AssetId> m_importedAssets;
    static AssetHandlerEditor *s_instance;
};

} // namespace Panda