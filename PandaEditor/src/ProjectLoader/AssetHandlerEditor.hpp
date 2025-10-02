#pragma once

#include <Panda/Assets/Base/Path.hpp>
#include <Panda/Assets/Base/AssetInfo.hpp>
#include <Rain/Coders/JsonDecoder.hpp>
#include <Rain/Coders/JsonEncoder.hpp>
#include <Panda/Assets/Base/AssetHandler.hpp>
#include <map>

namespace Panda {

class AssetHandlerEditor;

using RegisterAssetFunc = void (AssetHandlerEditor::*)(const path_t &path);

class AssetHandlerEditor : public AssetHandler {
public:
    AssetHandlerEditor();
    void registerAsset(const path_t &path);
    bool canImport(const path_t &path);
    void reload(AssetId id);
    AssetInfo getInfo(AssetId id);
    void updateInfo(AssetId assetId, const AssetInfo &assetInfo);
    UUID getAssetId(path_t path);
    void openProject(const path_t &path);
    void closeProject();
    const path_t &getProjectPath();

private:
    Asset *loadInternal(AssetId id, bool forcedReload = false) override;
    void registerTextureAsset(const path_t &path);
    void registerShaderAsset(const path_t &path);
    void registerMaterialAsset(const path_t &path);
    void loadAssetRegistry();
    void saveAssetRegistry();

    path_t m_projectPath;
    path_t m_assetRegistryPath;
    Rain::JsonEncoder m_jsonEncoder;
    Rain::JsonDecoder m_jsonDecoder;
    std::unordered_map<AssetId, AssetInfo> m_registry;
    std::unordered_map<path_t, AssetId> m_registeredAssets;
    std::map<path_t, RegisterAssetFunc> m_registerAssetFunc;
};

} // namespace Panda