#pragma once

#include <Miren/Base.hpp>

#include <string>

namespace Panda {

struct TextureAsset {
    Miren::TextureFormat m_format;
    uint16_t m_width;
    uint16_t m_height;
    Foundation::Memory m_data;

    Miren::TextureCreate getMirenTextureCreate() {
        Miren::TextureCreate create;
        create.m_data = m_data;
        create.m_format = m_format;
        create.m_numMips = 0;
        create.m_width = m_width;
        create.m_height = m_height;
        return create;
    }
};

struct ShaderAsset {
    const char * vertexCode;
    const char * fragmentCode;
};

class AssetLoader {
public:
    static TextureAsset loadTexure(const std::string &path);
    static ShaderAsset loadShader(const std::string &vertexPath, const std::string &fragmentPath);

    static void setResourcesPath(std::string path) {
        resourcesPath = path + "/";
    }

    static const std::string &getResourcesPath() {
        return resourcesPath;
    }

private:
    static std::string resourcesPath;
};
} // namespace Panda