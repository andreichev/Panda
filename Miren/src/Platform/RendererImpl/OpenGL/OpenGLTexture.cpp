//
// Created by Admin on 11.02.2022.
//

#include "Miren/PlatformData.hpp"
#include "OpenGLTexture.hpp"
#include "Texture/TextureFormat.hpp"

#include <Foundation/Assert.hpp>
#include <Foundation/Logger.hpp>
#include <Foundation/PlatformDetection.hpp>

#ifdef PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PLATFORM_DESKTOP)
#    include <glad/glad.h>
#endif

namespace Miren {

OpenGLTexture::OpenGLTexture()
    : m_id(-1)
    , m_format(TextureFormat::None) {}

void OpenGLTexture::create(const TextureCreate &create) {
    PND_ASSERT(m_id == -1, "TEXTURE ALREADY CREATED");
    m_format = create.m_format;
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    GLenum format = s_textureFormat[create.m_format].m_fmt;
    GLenum internalFormat = s_textureFormat[create.m_format].m_internalFmt;
    GLenum type = s_textureFormat[create.m_format].m_type;
    glTexImage2D(GL_TEXTURE_2D,
        0,
        format,
        create.m_width,
        create.m_height,
        0,
        internalFormat,
        type,
        create.m_data.data);
    create.m_data.release();

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
    // glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void OpenGLTexture::terminate() {
    PND_ASSERT(m_id != -1, "TEXTURE ALREADY DELETED");
    glDeleteTextures(1, &m_id);
    m_id = -1;
    m_format = TextureFormat::None;
}

void OpenGLTexture::bind(unsigned int slot) {
    PND_ASSERT(m_id != -1, "TEXTURE IS NOT CREATED");
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void OpenGLTexture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace Miren
