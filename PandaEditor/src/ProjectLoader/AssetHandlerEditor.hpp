#pragma once

#include <Panda/Assets/Base/Path.hpp>
#include <Panda/Assets/Base/AssetInfo.hpp>
#include <Rain/Coders/JsonDecoder.hpp>
#include <Rain/Coders/JsonEncoder.hpp>
#include <Panda/Assets/Base/AssetHandler.hpp>

namespace Panda {

class AssetHandlerEditor : public AssetHandler {
public:
    AssetHandlerEditor();
    Foundation::Shared<Asset> load(AssetId id) override;
    void registerTextureAsset(const path_t &path);
    UUID getAssetId(path_t path);
    void openProject(const path_t &path);
    void closeProject();

private:
    void loadAssetRegistry();
    void saveAssetRegistry();
    void unloadAllAssets();

    path_t m_projectPath;
    path_t m_assetRegistryPath;
    Rain::JsonEncoder m_jsonEncoder;
    Rain::JsonDecoder m_jsonDecoder;
    std::unordered_map<AssetId, AssetInfo> m_registry;
    std::unordered_map<path_t, AssetId> m_registeredAssets;
};

} // namespace Panda