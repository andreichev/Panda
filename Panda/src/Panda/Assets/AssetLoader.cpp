#include "Panda/Assets/AssetLoader.hpp"

#include <Foundation/Foundation.hpp>
#include <stb_image.h>

namespace Panda {

std::string AssetLoader::resourcesPath;

void releaseImage(void *data, void *userInfo) {
    stbi_image_free(data);
}

TextureAsset AssetLoader::loadTexure(const std::string &path) {
    // stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    std::string texturePath = AssetLoader::getResourcesPath() + path;
    unsigned char *image = stbi_load(texturePath.c_str(), &width, &height, &channels, 4);

    if (image == nullptr) {
        LOG_ERROR("Failed to load a texture file! {}", stbi_failure_reason());
    }

    TextureAsset texture;
    if (channels == 1) {
        texture.m_format = Miren::TextureFormat::RED_INTEGER;
    } else if (channels == 3) {
        texture.m_format = Miren::TextureFormat::RGB8;
    } else {
        texture.m_format = Miren::TextureFormat::RGBA8;
    }
    texture.m_width = width;
    texture.m_height = height;
    texture.m_data = Foundation::Memory(image);
    texture.m_data.releaseFn = &releaseImage;
    return texture;
}

ShaderAsset AssetLoader::loadShader(
    const std::string &vertexPath, const std::string &fragmentPath) {
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(AssetLoader::getResourcesPath() + vertexPath);
        fShaderFile.open(AssetLoader::getResourcesPath() + fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure &e) {
        PND_ASSERT_F(false, "SHADER::FILE {} or {} NOT SUCCESFULLY READ", vertexPath, fragmentPath);
    }
    char *vCode = (char *)malloc(vertexCode.size() + 1);
    char *fCode = (char *)malloc(fragmentCode.size() + 1);
    memcpy(vCode, vertexCode.c_str(), vertexCode.size() + 1);
    memcpy(fCode, fragmentCode.c_str(), fragmentCode.size() + 1);
    return {vCode, fCode};
}

} // namespace Panda