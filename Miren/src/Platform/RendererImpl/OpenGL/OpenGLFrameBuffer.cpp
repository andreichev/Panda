#include "OpenGLFrameBuffer.hpp"
#include "RendererOpenGL.hpp"

#include <Foundation/PlatformDetection.hpp>

#ifdef PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PLATFORM_DESKTOP)
#    include <glad/glad.h>
#endif

namespace Miren {

static inline bool isDepthFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::DEPTH24STENCIL8:
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
    : specification({})
    , m_id(-1) {}

void OpenGLFrameBuffer::create(FrameBufferSpecification specification) {
    PND_ASSERT(m_id == -1, "FRAMEBUFFER ALREADY CREATED");
    this->specification = specification;
    glCreateFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    for (int i = 0; i < specification.attachments.size(); i++) {
        FrameBufferAttachment &attach = specification.attachments[i];
        OpenGLTexture &texture = RendererOpenGL::s_instance->getTexture(attach.handle);
        int attachmentType;
        if (isDepthFormat(texture.getFormat())) {
            attachmentType = GL_DEPTH_ATTACHMENT;
        } else {
            attachmentType = GL_COLOR_ATTACHMENT0 + i;
        }
        GLuint textureId = texture.getId();
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, textureId, 0);
    }
    checkStatus();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFrameBuffer::resize(uint32_t width, uint32_t height) {
    // TODO: Implement
}

void OpenGLFrameBuffer::terminate() {
    PND_ASSERT(m_id != -1, "FRAMEBUFFER ALREADY DELETED");
    glDeleteFramebuffers(1, &m_id);
    m_id = 0;
}

} // namespace Miren