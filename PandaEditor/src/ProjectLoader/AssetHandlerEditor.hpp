#pragma once

#include <Panda/Assets/Base/Path.hpp>
#include <Panda/Assets/Base/AssetInfo.hpp>
#include <Rain/Coders/JsonDecoder.hpp>
#include <Rain/Coders/JsonEncoder.hpp>
#include <Panda/Assets/Base/AssetHandler.hpp>
#include <Panda/Renderer/MaterialData.hpp>
#include <map>

namespace Panda {

class AssetHandlerEditor;

using RegisterAssetFunc = void (AssetHandlerEditor::*)(const path_t &path);

using MissingFiles = std::array<std::string, 5>;

class AssetHandlerEditor final : public AssetHandler {
public:
    AssetHandlerEditor();
    void registerAsset(const path_t &path);
    bool canImport(const path_t &path);
    void reload(AssetId id);
    AssetInfo getInfo(AssetId id);
    void updateInfo(AssetId assetId, const AssetInfo &assetInfo);
    void openProject(const path_t &path);
    void closeProject();
    const path_t &getProjectPath();
    const std::map<AssetId, AssetInfo> &getRegistry();
    std::string getAssetName(const AssetInfo &info);
    bool assetFilesExist(AssetId id);
    MissingFiles getMissingAssetFiles(AssetId id);
    void locateMissingFiles(AssetId id, MissingFiles missingFiles);
    void removeAsset(AssetId id);
    bool saveMaterial(const AssetInfo &info, const MaterialData &materialData);

    UUID getAssetId(path_t path) override;
    std::unordered_set<path_t> getAssetPaths(AssetId id) override;

private:
    AssetInfo &getInfoRef(AssetId id);
    void registerTextureAsset(const path_t &path);
    void registerShaderAsset(const path_t &path);
    void registerMaterialAsset(const path_t &path);
    void loadAssetRegistry();
    void saveAssetRegistry();
    void removeMissingFiles(AssetId id);
    void addMissingFile(AssetId id, int index, const path_t &path);

    Asset *loadInternal(AssetId id, bool forcedReload = false) override;

    path_t m_projectPath;
    path_t m_assetRegistryPath;
    Rain::JsonEncoder m_jsonEncoder;
    Rain::JsonDecoder m_jsonDecoder;
    std::map<AssetId, AssetInfo> m_registry;
    // Получение assetId по пути. Нужно для функции getAssetId
    std::unordered_map<path_t, AssetId> m_registeredAssets;
    std::map<path_t, RegisterAssetFunc> m_registerAssetFunc;
    std::unordered_map<AssetId, MissingFiles> m_missingFiles;
};

} // namespace Panda