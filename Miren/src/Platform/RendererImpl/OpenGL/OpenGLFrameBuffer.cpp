#include "OpenGLFrameBuffer.hpp"

#include <Foundation/PlatformDetection.hpp>

#ifdef PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PLATFORM_DESKTOP)
#    include <glad/glad.h>
#endif

namespace Miren {

static GLenum getTextureTarget(bool multisampled) {
    return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

static inline bool isDepthFormat(FrameBufferTextureFormat format) {
    switch (format) {
        case FrameBufferTextureFormat::DEPTH24STENCIL8:
            return true;
        default:
            return false;
    }
}

void OpenGLFrameBuffer::checkStatus() {
    PND_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
        "Framebuffer is incomplete!");
}

OpenGLFrameBuffer::OpenGLFrameBuffer()
    : m_id(0)
    , m_depthAttachment(0) {}

void OpenGLFrameBuffer::create(FrameBufferSpecification specification) {
    this->specification = new FrameBufferSpecification(specification);
    reset();
}

void OpenGLFrameBuffer::reset() {
    if (m_id) {
        glDeleteFramebuffers(1, &m_id);
        glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
        if (m_depthAttachment) {
            glDeleteTextures(1, &m_depthAttachment);
        }
    }
    glCreateFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    for (FrameBufferAttachmentSpecification &spec : specification->attachments) {
        attachTexture(spec, specification->samples, specification->width, specification->height);
    }
    checkStatus();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFrameBuffer::resize(uint32_t width, uint32_t height) {
    PND_ASSERT(specification != nullptr, "FRAMEBUFFER SPECIFICATION IS UNDEFINED");
    specification->width = width;
    specification->height = height;
    reset();
}

void OpenGLFrameBuffer::terminate() {
    glDeleteFramebuffers(1, &m_id);
    glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
    if (m_depthAttachment) {
        glDeleteTextures(1, &m_depthAttachment);
    }
    m_colorAttachments.clear();
    m_depthAttachment = 0;
    delete specification;
    specification = nullptr;
    m_id = 0;
}

void OpenGLFrameBuffer::attachTexture(
    FrameBufferAttachmentSpecification &spec, int samples, uint32_t width, uint32_t height) {
    GLenum internalFormat;
    GLenum format;
    GLenum attachmentType;
    int index = m_colorAttachments.size();
    switch (spec.format) {
        case FrameBufferTextureFormat::RGBA8:
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
            attachmentType = GL_COLOR_ATTACHMENT0 + index;
            break;
        case FrameBufferTextureFormat::RED_INTEGER:
            internalFormat = GL_R32I;
            format = GL_RED_INTEGER;
            attachmentType = GL_COLOR_ATTACHMENT0 + index;
            break;
        case FrameBufferTextureFormat::DEPTH24STENCIL8:
            internalFormat = GL_DEPTH_STENCIL;
            format = GL_DEPTH24_STENCIL8;
            attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;
            break;
    }
    bool multisampled = samples > 1;
    bool isDepth = isDepthFormat(spec.format);
    GLuint textureId;
    glCreateTextures(getTextureTarget(multisampled), 1, &textureId);
    if (isDepth) {
        m_depthAttachment = textureId;
    } else {
        m_colorAttachments.emplace_back(textureId);
    }
    if (multisampled) {
        glTexImage2DMultisample(
            GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
    } else {
        if (isDepth) {
            glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
        } else {
            glTexImage2D(GL_TEXTURE_2D,
                0,
                internalFormat,
                width,
                height,
                0,
                format,
                GL_UNSIGNED_BYTE,
                nullptr);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, attachmentType, getTextureTarget(multisampled), textureId, 0);
}

} // namespace Miren