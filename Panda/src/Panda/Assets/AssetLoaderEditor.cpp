#include "Panda/Assets/AssetLoaderEditor.hpp"

#include <Miren/Miren.hpp>
#include <stb_image.h>
#include <fstream>
#include <sstream>

namespace Panda {

void releaseImage(void *data, void *userInfo) {
    stbi_image_free(data);
}

TextureData AssetLoaderEditor::loadTexture(const path_t &path) {
    // stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    void *image = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

    if (image == nullptr) {
        LOG_ERROR("Failed to load a texture file! %s", stbi_failure_reason());
        // TODO: Return "Missing" texture
        // https://ru.yougile.com/team/91006f9f80d3/#PANDA-41
        return {};
    }

    TextureData texture;
    if (channels == 1) {
        texture.m_format = Miren::TextureFormat::R32I;
    } else if (channels == 3) {
        texture.m_format = Miren::TextureFormat::RGB8;
    } else {
        texture.m_format = Miren::TextureFormat::RGBA8;
    }
    texture.m_width = width;
    texture.m_height = height;
    texture.m_data = Foundation::Memory(image);
    texture.m_data.releaseFn = &releaseImage;
    texture.m_wrap = Miren::TextureWrapMode::REPEAT;
    texture.m_isCubeMap = false;
    return texture;
}

TextureData AssetLoaderEditor::loadCubeMapTexture(std::array<path_t, 6> paths) {
    TextureData texture;
    std::array<void *, 6> images;
    int bytesPerColor;
    for (uint16_t side = 0; side < 6; ++side) {
        const path_t &texturePath = paths[side];
        int width, height, channels;
        void *image = stbi_load(texturePath.string().c_str(), &width, &height, &channels, 0);
        if (image == nullptr) {
            LOG_ERROR("Failed to load a texture file at path %s", texturePath.string().c_str());
            continue;
        }
        if (channels == 1) {
            texture.m_format = Miren::TextureFormat::R32I;
            bytesPerColor = 1;
        } else if (channels == 3) {
            texture.m_format = Miren::TextureFormat::RGB8;
            bytesPerColor = 3;
        } else {
            texture.m_format = Miren::TextureFormat::RGBA8;
            bytesPerColor = 4;
        }
        texture.m_width = width;
        texture.m_height = height;
        images[side] = image;
    }
    size_t imageSize = bytesPerColor * texture.m_width * texture.m_height;
    texture.m_data = Foundation::Memory::alloc(imageSize * 6);
    uint8_t *ptr = (uint8_t *)texture.m_data.data;
    for (uint16_t side = 0; side < 6; ++side) {
        memcpy(ptr, images[side], imageSize);
        ptr += imageSize;
        stbi_image_free(images[side]);
    }
    texture.m_wrap = Miren::TextureWrapMode::REPEAT;
    texture.m_isCubeMap = true;
    return texture;
}

ProgramData AssetLoaderEditor::loadProgram(const path_t &vertexPath, const path_t &fragmentPath) {
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
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
        PND_ASSERT_F(
            false,
            "SHADER::FILE {} or {} NOT SUCCESSFULLY READ",
            vertexPath.string().c_str(),
            fragmentPath.string().c_str()
        );
    }
    Foundation::Memory vertexData =
        Foundation::Memory::copying((void *)vertexCode.c_str(), vertexCode.size() + 1);
    Foundation::Memory fragmentData =
        Foundation::Memory::copying((void *)fragmentCode.c_str(), fragmentCode.size() + 1);
    return {vertexData, fragmentData};
}

} // namespace Panda