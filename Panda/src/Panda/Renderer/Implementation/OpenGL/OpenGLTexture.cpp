//
// Created by Admin on 11.02.2022.
//
#include "pndpch.hpp"
#include "OpenGLTexture.hpp"

#include <glad/glad.h>
#include <stb_image.h>

namespace Panda {

OpenGLTexture::OpenGLTexture(const char *path)
    : id(0) {
    glGenTextures(1, &id);

    /* Load image */
    // stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned char *image = stbi_load(path, &width, &height, &channels, 4);

    if (image == nullptr) {
        PND_ERROR("Failed to load a texture file! {}", stbi_failure_reason());
    }

    int format;
    if (channels == 1) {
        format = GL_RED;
    } else if (channels == 3) {
        format = GL_RGB;
    } else {
        format = GL_RGBA;
    }
    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
}

OpenGLTexture::~OpenGLTexture() {
    glDeleteTextures(1, &id);
}

void OpenGLTexture::bind(unsigned int slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}

void OpenGLTexture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace Panda