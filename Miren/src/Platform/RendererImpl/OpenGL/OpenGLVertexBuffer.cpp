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

OpenGLVertexBuffer::OpenGLVertexBuffer()
    : m_isDynamic(false)
    , m_id(-1) {}

void OpenGLVertexBuffer::terminate() {
    ASSERT(m_id != -1, "VERTEX BUFFER ALREADY DELETED");
    glDeleteBuffers(1, &m_id);
    m_id = -1;
}

void OpenGLVertexBuffer::create(void *data, uint32_t size, bool isDynamic) {
    ASSERT(m_id == -1, "VERTEX BUFFER ALREADY CREATED");
    m_isDynamic = isDynamic;
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    if (data != nullptr) {
        glBufferData(GL_ARRAY_BUFFER, size, data, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_layoutHandle = MIREN_INVALID_HANDLE;
}

void OpenGLVertexBuffer::update(void *data, uint32_t size) {
    ASSERT(m_id != -1, "VERTEX BUFFER NOT VALID");
    ASSERT(m_isDynamic != false, "Невозможно обновить статичный буфер");
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::bind() {
    ASSERT(m_id != -1, "VERTEX BUFFER NOT VALID");
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
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
