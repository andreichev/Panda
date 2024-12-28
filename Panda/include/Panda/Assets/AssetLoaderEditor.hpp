#pragma once

#include <Panda/Base/FileBase.hpp>
#include <Miren/Miren.hpp>

namespace Panda {

struct TextureData {
    Miren::TextureFormat m_format = Miren::RGBA8;
    Miren::TextureWrapMode m_wrap = Miren::REPEAT;
    uint16_t m_width = 1;
    uint16_t m_height = 1;
    Foundation::Memory m_data = nullptr;
    bool m_isCubeMap = false;

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

struct ProgramData {
    Foundation::Memory m_vertex;
    Foundation::Memory m_fragment;

    Miren::ProgramCreate getMirenProgramCreate() {
        return {m_vertex, m_fragment};
    }
};

class AssetLoaderEditor final {
public:
    static TextureData loadTexture(const path_t &path);
    static TextureData loadCubeMapTexture(std::array<path_t, 6> paths);
    static ProgramData loadProgram(const path_t &vertexPath, const path_t &fragmentPath);
};

} // namespace Panda