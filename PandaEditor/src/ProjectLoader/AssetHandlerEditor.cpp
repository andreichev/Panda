#include "AssetHandlerEditor.hpp"

#include <Panda/Assets/Base/AssetImporterBase.hpp>
#include <Panda/Assets/TextureAsset.hpp>
#include <Panda/Assets/GpuProgramAsset.hpp>

#include <fstream>

namespace Panda {

AssetHandlerEditor::AssetHandlerEditor()
    : m_projectPath()
    , m_assetRegistryPath()
    , m_registry()
    , m_jsonEncoder(true)
    , m_jsonDecoder()
    , m_registeredAssets() {}

Foundation::Shared<Asset> AssetHandlerEditor::load(AssetId id) {
    if (m_registry.find(id) == m_registry.end()) {
        PND_ASSERT(false, "UNKNOWN ASSET ID");
        return nullptr;
    }
    if (m_cache.find(id) != m_cache.end()) {
        auto asset = m_cache.at(id).lock();
        if (asset) { return asset; }
    }
    auto assetInfo = m_registry.at(id);
    Foundation::Shared<Asset> asset;
    switch (assetInfo.type) {
        case AssetType::TEXTURE: {
            auto meta = std::get<TextureAssetMeta>(assetInfo.meta);
            path_t path = m_projectPath / meta.path;
            if (std::filesystem::exists(path)) {
                Miren::TextureCreate create = AssetImporterBase::load2DTexture(path);
                create.m_minFiltering = meta.minFiltering;
                create.m_magFiltering = meta.magFiltering;
                create.m_numMips = meta.numMips;
                asset = Foundation::makeShared<TextureAsset>(create);
                LOG_INFO("CREATED TEXTURE: %u", id);
            } else {
                Foundation::Memory violetTextureData = Foundation::Memory::alloc(sizeof(uint32_t));
                *(uint32_t *)violetTextureData.data = 0xFFFF008F;
                Miren::TextureCreate violetTextureCreate;
                violetTextureCreate.m_data = violetTextureData;
                asset = Foundation::makeShared<TextureAsset>(violetTextureCreate);
            }
            break;
        }
        case AssetType::PROGRAM: {
            auto meta = std::get<GpuProgramAssetMeta>(assetInfo.meta);
            asset = Foundation::makeShared<GpuProgramAsset>(
                m_projectPath / meta.vertexCodePath, m_projectPath / meta.fragmentCodePath
            );
            LOG_INFO("CREATED GPU PROGRAM: %u", id);
            break;
        }
        case AssetType::CUBE_MAP: {
            asset = nullptr;
            break;
        }
        case AssetType::NONE: {
            asset = nullptr;
            break;
        }
    }
    m_cache[id] = asset;
    return asset;
}

void AssetHandlerEditor::registerTextureAsset(const path_t &path) {
    PND_ASSERT(!getAssetId(path), "ASSET ALREADY IMPORTED");
    path_t assetPath = std::filesystem::relative(path, m_projectPath);
    AssetInfo info;
    info.id = UUID();
    info.type = AssetType::TEXTURE;
    TextureAssetMeta meta;
    meta.minFiltering = Miren::NEAREST;
    meta.magFiltering = Miren::NEAREST;
    meta.path = assetPath;
    info.meta = meta;
    m_registry[info.id] = info;
    m_registeredAssets[assetPath] = info.id;
    saveAssetRegistry();
}

UUID AssetHandlerEditor::getAssetId(path_t path) {
    path_t assetPath = std::filesystem::relative(path, m_projectPath);
    if (m_registeredAssets.find(assetPath) == m_registeredAssets.end()) { return 0; }
    return m_registeredAssets[assetPath];
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
        for (auto &info : registryDto.assets) {
            m_registry[info.id] = info;
            switch (info.type) {
                case AssetType::TEXTURE: {
                    auto meta = std::get<TextureAssetMeta>(info.meta);
                    if (!std::filesystem::exists(m_projectPath / meta.path)) {
                        LOG_ERROR_EDITOR("Texture asset %s not found.", meta.path.string().c_str());
                    }
                    m_registeredAssets[meta.path] = info.id;
                    break;
                }
                case AssetType::PROGRAM: {
                    auto meta = std::get<GpuProgramAssetMeta>(info.meta);
                    m_registeredAssets[meta.vertexCodePath] = info.id;
                    m_registeredAssets[meta.fragmentCodePath] = info.id;
                    break;
                }
                default:
                    break;
            }
        }
    } else {
        LOG_INFO("ASSET REGISTRY NOT FOUND");
    }
}

void AssetHandlerEditor::closeProject() {
    saveAssetRegistry();
    m_registry.clear();
    m_assetRegistryPath.clear();
    m_projectPath.clear();
}

void AssetHandlerEditor::saveAssetRegistry() {
    std::ofstream file(m_assetRegistryPath);
    if (file.is_open()) {
        AssetRegistryDto registryDto;
        for (auto &entry : m_registry) {
            registryDto.assets.push_back(entry.second);
        }
        Rain::Encoder *encoder = &m_jsonEncoder;
        encoder->encode(file, registryDto);
        file.close();
        LOG_INFO_EDITOR("Asset registry saved.");
    } else {
        LOG_INFO("ASSET REGISTRY SAVING ERROR");
    }
}

} // namespace Panda