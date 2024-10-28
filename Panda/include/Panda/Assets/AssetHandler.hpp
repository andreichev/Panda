#pragma once

#include "Asset.hpp"
#include "Panda/Serialization/FileStream.hpp"

#include <Miren/Base.hpp>
#include <string>
#include <filesystem>

namespace Panda {

using path_t = std::filesystem::path;

struct AssetPackFileInfo {
    uint64_t size;
    uint64_t offset;
    AssetType type;
};

class AssetHandler {
public:
    virtual ~AssetHandler() = default;
    virtual Foundation::Shared<Asset> loadEditor(const path_t &filePath) = 0;
    virtual void saveRuntime(const FileStreamReader &fileStream, AssetId id) {
        LOG_CRITICAL("NOT IMPLEMENTED");
    }
    virtual Foundation::Shared<Asset> loadRuntime(const FileStreamReader &fileStream, AssetPackFileInfo info) {
        LOG_CRITICAL("NOT IMPLEMENTED");
    }
};

struct TextureAsset {
    Miren::TextureFormat m_format;
    Miren::TextureWrapMode m_wrap;
    uint16_t m_width;
    uint16_t m_height;
    Foundation::Memory m_data;
    bool m_isCubeMap;

    Miren::TextureCreate getMirenTextureCreate() {
        Miren::TextureCreate create;
        create.m_data = m_data;
        create.m_format = m_format;
        create.m_wrap = m_wrap;
        create.m_width = m_width;
        create.m_height = m_height;
        create.m_isCubeMap = m_isCubeMap;
        return create;
    }
};

struct ProgramAsset {
    Foundation::Memory m_vertex;
    Foundation::Memory m_fragment;

    Miren::ProgramCreate getMirenProgramCreate() {
        return {m_vertex, m_fragment};
    }
};

} // namespace Panda