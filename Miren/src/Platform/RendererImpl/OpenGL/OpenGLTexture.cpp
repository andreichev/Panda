//
// Created by Admin on 11.02.2022.
//

#include "Miren/PlatformData.hpp"
#include "OpenGLTexture.hpp"

#include <Foundation/Assert.hpp>
#include <Foundation/Logger.hpp>
#include <Foundation/PlatformDetection.hpp>

#ifdef PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PLATFORM_DESKTOP)
#    include <glad/glad.h>
#endif
#include <stb_image.h>

namespace Miren {

OpenGLTexture::OpenGLTexture()
    : m_id(-1) {}

void OpenGLTexture::create(void *pixels, uint32_t width, uint32_t height) {
    ASSERT(m_id == -1, "TEXTURE ALREADY CREATED");
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
#ifdef GL_UNPACK_ROW_LENGTH
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void OpenGLTexture::create(const char *path) {
    ASSERT(m_id == -1, "TEXTURE ALREADY CREATED");
    glGenTextures(1, &m_id);
    /* Load image */
    // stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    std::string texturePath = PlatformData::get()->getResourcesPath() + path;
    unsigned char *image = stbi_load(texturePath.c_str(), &width, &height, &channels, 4);

    if (image == nullptr) {
        LOG_ERROR("Failed to load a texture file! {}", stbi_failure_reason());
    }

    int format;
    if (channels == 1) {
        format = GL_RED;
    } else if (channels == 3) {
        format = GL_RGB;
    } else {
        format = GL_RGBA;
    }
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
}

void OpenGLTexture::terminate() {
    ASSERT(m_id != -1, "TEXTURE ALREADY DELETED");
    glDeleteTextures(1, &m_id);
    m_id = -1;
}

void OpenGLTexture::bind(unsigned int slot) {
    ASSERT(m_id != -1, "TEXTURE IS NOT CREATED");
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void OpenGLTexture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace Miren
