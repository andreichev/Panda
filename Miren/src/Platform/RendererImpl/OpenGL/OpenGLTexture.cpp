//
// Created by Admin on 11.02.2022.
//

#include "Miren/PlatformData.hpp"
#include "OpenGLTexture.hpp"
#include "Texture/TextureFormat.hpp"

#include "OpenGLBase.hpp"

namespace Miren {

OpenGLTexture::OpenGLTexture()
    : m_id(-1)
    , m_create() {}

void OpenGLTexture::create(const TextureCreate &create) {
    PND_ASSERT(m_id == -1, "TEXTURE ALREADY CREATED");
    m_create = create;
    MIREN_LOG("CREATE TEXTURE, w: {}, h: {}", create.m_width, create.m_height);
    GL_CALL(glGenTextures(1, &m_id));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, m_id));

    GLenum format = s_textureFormat[create.m_format].m_fmt;
    GLenum internalFormat = s_textureFormat[create.m_format].m_internalFmt;
    GLenum type = s_textureFormat[create.m_format].m_type;
    GL_CALL(glTexImage2D(GL_TEXTURE_2D,
        0,
        format,
        create.m_width,
        create.m_height,
        0,
        internalFormat,
        type,
        create.m_data.data));
    create.m_data.release();

    if (create.m_numMips > 0) {
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, create.m_numMips));
        GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
    }

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CALL(glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, openGLFiltering(create.m_minFiltering)));
    GL_CALL(glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, openGLFiltering(create.m_magFiltering)));
}

void OpenGLTexture::resize(uint32_t width, uint32_t height) {
    PND_ASSERT(m_id != -1, "TEXTURE IS NOT CREATED");
    terminate();
    m_create.m_width = width;
    m_create.m_height = height;
    create(m_create);
}

void OpenGLTexture::terminate() {
    PND_ASSERT(m_id != -1, "TEXTURE ALREADY DELETED");
    GL_CALL(glDeleteTextures(1, &m_id));
    m_id = -1;
}

void OpenGLTexture::bind(unsigned int slot) {
    PND_ASSERT(m_id != -1, "TEXTURE IS NOT CREATED");
    GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, m_id));
}

void OpenGLTexture::unbind() {
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

} // namespace Miren
