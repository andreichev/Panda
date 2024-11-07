#include "AssetHandlerEditor.hpp"

#include <Panda/Assets/AssetLoaderEditor.hpp>
#include <Panda/Renderer/Texture.hpp>
#include <Panda/Renderer/GpuProgram.hpp>

#include <fstream>

namespace Panda {

AssetHandlerEditor::AssetHandlerEditor()
    : m_projectPath()
    , m_assetRegistryPath()
    , m_registry()
    , m_cache()
    , m_jsonEncoder(true)
    , m_jsonDecoder()
    , m_importedAssets() {}

Foundation::Shared<Asset> AssetHandlerEditor::load(AssetId id) {
    PND_ASSERT(m_registry.find(id) != m_registry.end(), "UNKNOWN ASSET ID");
    if (m_cache.find(id) != m_cache.end()) {
        return m_cache.at(id);
    }
    auto assetInfo = m_registry.at(id);
    Foundation::Shared<Asset> asset;
    switch (assetInfo.type) {
        case AssetType::TEXTURE:
            asset = Foundation::makeShared<Texture>(assetInfo.path);
        case AssetType::PROGRAM:
            asset = nullptr; // Foundation::makeShared<GpuProgram>(assetInfo.path);
        case AssetType::CUBE_MAP:
            asset = nullptr;
        case AssetType::NONE:
            asset = nullptr;
    }
    m_cache[id] = asset;
    return asset;
}

void AssetHandlerEditor::importAsset(const path_t &path) {
    PND_ASSERT(!isAssetImported(path), "ASSET ALREADY IMPORTED");
    path_t assetPath = std::filesystem::relative(path, m_projectPath);
    AssetInfoEditor info;
    info.id = UUID();
    info.path = assetPath;
    info.type = AssetType::TEXTURE;
    m_registry[info.id] = info;
    m_importedAssets.insert(assetPath);
    saveAssetRegistry();
}

bool AssetHandlerEditor::isAssetImported(path_t path) {
    path_t assetPath = std::filesystem::relative(path, m_projectPath);
    return m_importedAssets.contains(assetPath);
}

void AssetHandlerEditor::openProject(const path_t &path) {
    m_projectPath = path;
    path_t projectName = m_projectPath.filename();
    path_t pandaDirectoryPath = path;
    pandaDirectoryPath.append(".Panda");
    if (!std::filesystem::exists(pandaDirectoryPath)) {
        std::filesystem::create_directory(pandaDirectoryPath);
    }
    m_assetRegistryPath = pandaDirectoryPath;
    m_assetRegistryPath.append("assets.json");
    loadAssetRegistry();
}

void AssetHandlerEditor::loadAssetRegistry() {
    std::ifstream file(m_assetRegistryPath);
    if (file.is_open()) {
        AssetRegistryDto registryDto;
        Rain::Decoder *decoder = &m_jsonDecoder;
        decoder->decode(file, registryDto);
        file.close();
        for (auto &assetInfoDto : registryDto.assets) {
            AssetInfoEditor assetInfo = AssetRegistryMapper::toEntity(assetInfoDto);
            m_registry[assetInfo.id] = assetInfo;
            m_importedAssets.insert(assetInfo.path);
        }
    } else {
        LOG_INFO("ASSET REGISTRY NOT FOUND");
    }
}

void AssetHandlerEditor::closeProject() {
    saveAssetRegistry();
    m_registry.clear();
    m_cache.clear();
    m_assetRegistryPath.clear();
    m_projectPath.clear();
}

void AssetHandlerEditor::saveAssetRegistry() {
    std::ofstream file(m_assetRegistryPath);
    if (file.is_open()) {
        AssetRegistryDto registryDto;
        for (auto &entry : m_registry) {
            AssetInfoEditorDto assetInfoDto = AssetRegistryMapper::toDto(entry.second);
            registryDto.assets.push_back(assetInfoDto);
        }
        Rain::Encoder *encoder = &m_jsonEncoder;
        encoder->encode(file, registryDto);
        file.close();
        LOG_EDITOR("Asset registry saved.");
    } else {
        LOG_INFO("ASSET REGISTRY SAVING ERROR");
    }
}

} // namespace Panda