#include "AssetHandlerEditor.hpp"

#include <fstream>

namespace Panda {

AssetHandlerEditor::AssetHandlerEditor()
    : m_projectPath()
    , m_assetRegistryPath()
    , m_registry()
    , m_cache()
    , m_jsonEncoder(true)
    , m_jsonDecoder() {}

Foundation::Shared<Asset> AssetHandlerEditor::load(AssetId id) {}

void AssetHandlerEditor::openProject(const path_t path) {
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
    } else {
        LOG_INFO("ASSET REGISTRY SAVING ERROR");
    }
}

} // namespace Panda