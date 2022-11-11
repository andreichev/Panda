//
// Created by Admin on 11.02.2022.
//

#include "OpenGLVertexBuffer.hpp"

#include <Foundation/Assert.hpp>
#include <Foundation/Logger.hpp>
#include <Foundation/PlatformDetection.hpp>

#ifdef PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PLATFORM_DESKTOP)
#    include <glad/glad.h>
#endif

namespace Miren {

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    glDeleteBuffers(1, &id);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(void *data, uint32_t size, bool isDynamic)
    : isDynamic(isDynamic)
    , id(0) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    if (data != nullptr) {
        glBufferData(GL_ARRAY_BUFFER, size, data, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_layoutHandle = MIREN_INVALID_HANDLE;
}

void OpenGLVertexBuffer::update(void *data, uint32_t size) {
    if (isDynamic == false) {
        LOG_CRITICAL("Невозможно обновить статичный буфер");
    }
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void OpenGLVertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::setLayoutHandle(VertexLayoutHandle layoutHandle) {
    m_layoutHandle = layoutHandle;
}

VertexLayoutHandle OpenGLVertexBuffer::getLayoutHandle() {
    return m_layoutHandle;
}

} // namespace Miren
