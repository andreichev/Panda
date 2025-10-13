#include "AssetHandlerEditor.hpp"
#include "Assets/EditorShaderAsset.hpp"

#include <Panda/Assets/Base/AssetImporterBase.hpp>
#include <Panda/Assets/Fallback/FallbackAssets.hpp>
#include <Panda/Assets/MaterialAsset.hpp>
#include <Panda/Serialization/AssetsMapper.hpp>

#include <fstream>

namespace Panda {

AssetHandlerEditor::AssetHandlerEditor()
    : m_projectPath()
    , m_assetRegistryPath()
    , m_registry()
    , m_jsonEncoder(true)
    , m_jsonDecoder()
    , m_registeredAssets()
    , m_registerAssetFunc()
    , AssetHandler(Foundation::getAllocator()) {
    m_registerAssetFunc.emplace(".png", &AssetHandlerEditor::registerTextureAsset);
    m_registerAssetFunc.emplace(".jpeg", &AssetHandlerEditor::registerTextureAsset);
    m_registerAssetFunc.emplace(".hlsl", &AssetHandlerEditor::registerShaderAsset);
    m_registerAssetFunc.emplace(".mat", &AssetHandlerEditor::registerMaterialAsset);
}

void AssetHandlerEditor::reload(AssetId id) {
    loadInternal(id, true);
}

Asset *AssetHandlerEditor::loadInternal(AssetId id, bool forcedReload) {
    if (!forcedReload) {
        if (m_loadedAssets.find(id) != m_loadedAssets.end()) { return m_loadedAssets.at(id).asset; }
    }
    if (m_registry.find(id) == m_registry.end()) {
        PND_ASSERT_F(false, "UNKNOWN ASSET ID %u", id);
        return nullptr;
    }
    auto assetInfo = m_registry.at(id);
    Asset *asset;
    switch (assetInfo.type) {
        case AssetType::TEXTURE: {
            auto meta = std::get<TextureAssetMeta>(assetInfo.meta);
            path_t path = m_projectPath / meta.path;
            if (std::filesystem::exists(path)) {
                Miren::TextureCreate create = AssetImporterBase::load2DTexture(path);
                create.m_minFiltering = meta.minFiltering;
                create.m_magFiltering = meta.magFiltering;
                create.m_numMips = meta.numMips;
                asset = F_NEW(m_allocator, TextureAsset)(create);
                LOG_INFO("CREATED TEXTURE %u AT PATH %s", id, meta.path.string().c_str());
                LOG_INFO_EDITOR("CREATED TEXTURE %u AT PATH %s", id, meta.path.string().c_str());
            } else {
                asset = F_NEW(m_allocator, FallbackTextureAsset)();
                LOG_ERROR(
                    "TEXTURE ASSET %u NOT FOUND AT PATH %s: ", id, meta.path.string().c_str()
                );
                LOG_ERROR_EDITOR(
                    "TEXTURE ASSET %u NOT FOUND AT PATH %s: ", id, meta.path.string().c_str()
                );
            }
            break;
        }
        case AssetType::SHADER: {
            auto meta = std::get<ShaderAssetMeta>(assetInfo.meta);
            path_t vertPath = m_projectPath / meta.vertexCodePath;
            path_t fragPath = m_projectPath / meta.fragmentCodePath;
            if (std::filesystem::exists(vertPath) && std::filesystem::exists(fragPath)) {
                asset = F_NEW(m_allocator, EditorShaderAsset)(vertPath, fragPath);
                LOG_INFO(
                    "CREATED SHADER %u. VERT: %s, FRAG: %s",
                    id,
                    vertPath.string().c_str(),
                    fragPath.string().c_str()
                );
                LOG_INFO_EDITOR(
                    "CREATED SHADER %u. VERT: %s, FRAG: %s",
                    id,
                    vertPath.string().c_str(),
                    fragPath.string().c_str()
                );
            } else {
                asset = F_NEW(m_allocator, ShaderAsset)();
                if (!std::filesystem::exists(vertPath)) {
                    LOG_ERROR(
                        "SHADER ASSET %u NOT FOUND AT PATH %s: ", id, vertPath.string().c_str()
                    );
                    LOG_ERROR_EDITOR(
                        "SHADER ASSET %u NOT FOUND AT PATH %s: ", id, vertPath.string().c_str()
                    );
                }
                if (!std::filesystem::exists(fragPath)) {
                    LOG_ERROR(
                        "SHADER ASSET %u NOT FOUND AT PATH %s: ", id, fragPath.string().c_str()
                    );
                    LOG_ERROR_EDITOR(
                        "SHADER ASSET %u NOT FOUND AT PATH %s: ", id, fragPath.string().c_str()
                    );
                }
            }
            break;
        }
        case AssetType::MATERIAL: {
            auto meta = std::get<MaterialAssetMeta>(assetInfo.meta);
            MaterialDataDto dataDto;
            std::ifstream file(m_projectPath / meta.materialPath);
            if (file.is_open()) {
                Rain::Decoder *decoder = &m_jsonDecoder;
                decoder->decode(file, dataDto);
                file.close();
                AssetRef<ShaderAsset> shader;
                if (meta.shader) { shader = makeRef<ShaderAsset>(meta.shader); }
                MaterialData data;
                AssetsMapper::toData(data, dataDto);
                asset = F_NEW(m_allocator, MaterialAsset)(data, shader);
                LOG_INFO("CREATED MATERIAL %u AT PATH %s", id, meta.materialPath.string().c_str());
                LOG_INFO_EDITOR(
                    "CREATED MATERIAL %u AT PATH %s", id, meta.materialPath.string().c_str()
                );
            } else {
                LOG_ERROR("MATERIAL %u FILE %s NOT FOUND", id, meta.materialPath.string().c_str());
                LOG_ERROR_EDITOR(
                    "MATERIAL %u FILE %s NOT FOUND", id, meta.materialPath.string().c_str()
                );
                asset = nullptr;
            }
            break;
        }
        case AssetType::CUBE_MAP:
        case AssetType::MESH:
        case AssetType::NONE: {
            asset = nullptr;
            break;
        }
    }
    PND_ASSERT(asset != nullptr, "ASSET IS NOT LOADED");
    if (forcedReload && m_loadedAssets.find(id) != m_loadedAssets.end()) {
        Asset *prevAsset = m_loadedAssets.at(id).asset;
        F_FREE(m_allocator, prevAsset);
    }
    AssetEntry entry;
    entry.asset = asset;
    m_loadedAssets[id] = entry;
    return asset;
}

void AssetHandlerEditor::registerTextureAsset(const path_t &path) {
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

void AssetHandlerEditor::registerShaderAsset(const path_t &path) {
    path_t assetPath = std::filesystem::relative(path, m_projectPath);
    AssetInfo info;
    info.id = UUID();
    info.type = AssetType::SHADER;
    ShaderAssetMeta meta;
    meta.vertexCodePath = replaceLastChars(assetPath, "vert.hlsl");
    meta.fragmentCodePath = replaceLastChars(assetPath, "frag.hlsl");
    info.meta = meta;
    m_registry[info.id] = info;
    m_registeredAssets[meta.vertexCodePath] = info.id;
    m_registeredAssets[meta.fragmentCodePath] = info.id;
    saveAssetRegistry();
}

void AssetHandlerEditor::registerMaterialAsset(const path_t &path) {
    path_t assetPath = std::filesystem::relative(path, m_projectPath);
    AssetInfo info;
    info.id = UUID();
    info.type = AssetType::MATERIAL;
    MaterialAssetMeta meta;
    meta.materialPath = assetPath;
    meta.shader = 0;
    info.meta = meta;
    m_registry[info.id] = info;
    m_registeredAssets[assetPath] = info.id;
    saveAssetRegistry();
}

void AssetHandlerEditor::registerAsset(const path_t &path) {
    path_t extension = path.extension();
    if (!canImport(path)) {
        LOG_ERROR_EDITOR("EXTENSION %s IMPORT NOT SUPPORTED YET", extension.c_str());
        return;
    }
    RegisterAssetFunc registerAssetFunc = m_registerAssetFunc[extension];
    (this->*registerAssetFunc)(path);
}

bool AssetHandlerEditor::canImport(const path_t &path) {
    PND_ASSERT(!getAssetId(path), "ASSET ALREADY IMPORTED");
    path_t extension = path.extension();
    return m_registerAssetFunc.find(extension) != m_registerAssetFunc.end();
}

AssetInfo AssetHandlerEditor::getInfo(AssetId id) {
    if (m_registry.find(id) == m_registry.end()) {
        PND_ASSERT(false, "UNKNOWN ASSET ID");
        return {};
    }
    return m_registry.at(id);
}

void AssetHandlerEditor::updateInfo(AssetId id, const AssetInfo &assetInfo) {
    if (m_registry.find(id) == m_registry.end()) {
        PND_ASSERT(false, "UNKNOWN ASSET ID");
        return;
    }
    PND_ASSERT(assetInfo.id == id, "ASSET ID DOES NOT MATCH");
    AssetInfo &prevInfo = m_registry.at(id);
    PND_ASSERT(prevInfo.type == assetInfo.type, "CAN'T CHANGE ASSET TYPE");
    m_registry[id] = assetInfo;
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
    m_registry.clear();
    m_registeredAssets.clear();
    std::ifstream file(m_assetRegistryPath);
    if (file.is_open()) {
        AssetRegistryDto registryDto;
        Rain::Decoder *decoder = &m_jsonDecoder;
        decoder->decode(file, registryDto);
        file.close();
        for (auto &info : registryDto.assets) {
            switch (info.type) {
                case AssetType::TEXTURE: {
                    auto meta = std::get<TextureAssetMeta>(info.meta);
                    if (meta.path.empty()) {
                        LOG_ERROR_EDITOR(
                            "Empty asset path. Skipping broken asset", meta.path.string().c_str()
                        );
                        break;
                    }
                    if (!std::filesystem::exists(m_projectPath / meta.path)) {
                        LOG_ERROR_EDITOR("Texture asset %s not found.", meta.path.string().c_str());
                    }
                    m_registeredAssets[meta.path] = info.id;
                    m_registry[info.id] = info;
                    break;
                }
                case AssetType::SHADER: {
                    auto meta = std::get<ShaderAssetMeta>(info.meta);
                    if (meta.fragmentCodePath.empty()) {
                        LOG_ERROR_EDITOR(
                            "Empty asset path. Skipping broken asset",
                            meta.fragmentCodePath.string().c_str()
                        );
                        break;
                    }
                    if (!std::filesystem::exists(m_projectPath / meta.vertexCodePath)) {
                        LOG_ERROR_EDITOR(
                            "Shader asset %s not found.", meta.vertexCodePath.string().c_str()
                        );
                    }
                    if (!std::filesystem::exists(m_projectPath / meta.fragmentCodePath)) {
                        LOG_ERROR_EDITOR(
                            "Shader asset %s not found.", meta.fragmentCodePath.string().c_str()
                        );
                    }
                    m_registeredAssets[meta.vertexCodePath] = info.id;
                    m_registeredAssets[meta.fragmentCodePath] = info.id;
                    m_registry[info.id] = info;
                    break;
                }
                case AssetType::MATERIAL: {
                    auto meta = std::get<MaterialAssetMeta>(info.meta);
                    if (meta.materialPath.empty()) {
                        LOG_ERROR_EDITOR(
                            "Empty asset path. Skipping broken asset",
                            meta.materialPath.string().c_str()
                        );
                        break;
                    }
                    if (!std::filesystem::exists(m_projectPath / meta.materialPath)) {
                        LOG_ERROR_EDITOR(
                            "Material asset %s not found.", meta.materialPath.string().c_str()
                        );
                    }
                    m_registeredAssets[meta.materialPath] = info.id;
                    m_registry[info.id] = info;
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
        LOG_INFO("Asset registry saved.");
        LOG_INFO_EDITOR("Asset registry saved.");
    } else {
        LOG_ERROR("ASSET REGISTRY SAVING ERROR");
        LOG_ERROR_EDITOR("ASSET REGISTRY SAVING ERROR");
    }
}

const path_t &AssetHandlerEditor::getProjectPath() {
    return m_projectPath;
}

std::unordered_set<path_t> AssetHandlerEditor::getAssetPaths(AssetId id) {
    if (m_registry.find(id) == m_registry.end()) { return {}; }
    AssetInfo info = getInfo(id);
    std::unordered_set<path_t> result;
    switch (info.type) {
        case AssetType::TEXTURE: {
            TextureAssetMeta meta = std::get<TextureAssetMeta>(info.meta);
            result.emplace(m_projectPath / meta.path);
            break;
        }
        case AssetType::SHADER: {
            ShaderAssetMeta meta = std::get<ShaderAssetMeta>(info.meta);
            result.emplace(m_projectPath / meta.vertexCodePath);
            result.emplace(m_projectPath / meta.fragmentCodePath);
            break;
        }
        case AssetType::MATERIAL: {
            MaterialAssetMeta meta = std::get<MaterialAssetMeta>(info.meta);
            result.emplace(m_projectPath / meta.materialPath);
            break;
        }
        case AssetType::CUBE_MAP:
        case AssetType::MESH:
        case AssetType::NONE: {
            break;
        }
    }
    return result;
}

const std::unordered_map<AssetId, AssetInfo> &AssetHandlerEditor::getRegistry() {
    return m_registry;
}

} // namespace Panda