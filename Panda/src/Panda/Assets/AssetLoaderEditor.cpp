#include "Panda/Assets/AssetLoaderEditor.hpp"

#include <Foundation/PlatformDetection.hpp>
#include <Miren/Miren.hpp>
#include <Fern/Fern.hpp>
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
#ifdef PLATFORM_ANDROID
    auto inputStream = Fern::createStaticResourceReader(path);
    size_t fileSize = inputStream->totalSize();
    PND_ASSERT(fileSize != 0, "UNKNOWN ASSET SIZE");
    Foundation::Memory textureMem = Foundation::Memory::alloc(fileSize);
    inputStream->readData(textureMem.data, fileSize);
    void *image =
        stbi_load_from_memory((uint8_t *)textureMem.data, fileSize, &width, &height, &channels, 0);
    textureMem.release();
    Fern::disposeResourceReader(inputStream);
#else
    void *image = stbi_load(
        (Fern::getStaticResourcesPath() / path).string().c_str(), &width, &height, &channels, 0
    );
#endif

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
        int width, height, channels;
        const path_t &texturePath = paths[side];
#ifdef PLATFORM_ANDROID
        auto inputStream = Fern::createStaticResourceReader(texturePath);
        size_t fileSize = inputStream->totalSize();
        PND_ASSERT(fileSize != 0, "UNKNOWN ASSET SIZE");
        Foundation::Memory textureMem = Foundation::Memory::alloc(fileSize);
        inputStream->readData(textureMem.data, fileSize);
        void *image = stbi_load_from_memory(
            (uint8_t *)textureMem.data, fileSize, &width, &height, &channels, 0
        );
        textureMem.release();
        Fern::disposeResourceReader(inputStream);
#else
        void *image = stbi_load(
            (Fern::getStaticResourcesPath() / texturePath).string().c_str(),
            &width,
            &height,
            &channels,
            0
        );
#endif
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
#ifndef PLATFORM_ANDROID
    // TODO: Добавить пустой рабочий шейдер в случае отсутствия файла или ошибки компиляции
    if (!std::filesystem::exists(vertexPath)) {
        LOG_ERROR("FILE %s DOESN'T EXISTS", vertexPath.string().c_str());
    }
    if (!std::filesystem::exists(fragmentPath)) {
        LOG_ERROR("FILE %s DOESN'T EXISTS", vertexPath.string().c_str());
    }
#endif
    const path_t fullVertexPath = Fern::getStaticResourcesPath() / vertexPath;
    const path_t fullFragmentPath = Fern::getStaticResourcesPath() / fragmentPath;
    // ----------READ VERTEX SHADER ----------
    size_t fileSize;
    auto inputStream = Fern::createStaticResourceReader(fullVertexPath);
    fileSize = inputStream->totalSize() + 1;
    PND_ASSERT(fileSize != 0, "UNKNOWN ASSET SIZE");
    Foundation::Memory vertexData = Foundation::Memory::alloc(fileSize);
    ((uint8_t *)vertexData.data)[fileSize - 1] = 0;
    inputStream->readData(vertexData.data, fileSize);
    Fern::disposeResourceReader(inputStream);
    // ----------READ FRAGMENT SHADER ---------
    inputStream = Fern::createStaticResourceReader(fullFragmentPath);
    fileSize = inputStream->totalSize() + 1;
    PND_ASSERT(fileSize != 0, "UNKNOWN ASSET SIZE");
    Foundation::Memory fragmentData = Foundation::Memory::alloc(fileSize);
    inputStream->readData(fragmentData.data, fileSize);
    ((uint8_t *)fragmentData.data)[fileSize - 1] = 0;
    Fern::disposeResourceReader(inputStream);
    // ----------------------------------------
    return {vertexData, fragmentData};
}

} // namespace Panda
