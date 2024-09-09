#pragma once

#include <Miren/Base.hpp>

#include <string>
#include <filesystem>

namespace Panda {

using path_t = std::filesystem::path;

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

class AssetLoader {
public:
    static TextureAsset loadTexture(const path_t &path);
    static TextureAsset loadCubeMapTexture(std::array<path_t, 6> paths);
    static ProgramAsset loadProgram(const path_t &vertexPath, const path_t &fragmentPath);

    static void setResourcesPath(std::string path) {
        resourcesPath = path;
    }

    static const path_t &getResourcesPath() {
        return resourcesPath;
    }

private:
    static path_t resourcesPath;
};
} // namespace Panda