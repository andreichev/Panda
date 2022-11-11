#include "OpenGLVertexLayout.hpp"

#include <Foundation/Logger.hpp>
#include <Foundation/PlatformDetection.hpp>

#ifdef PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PLATFORM_DESKTOP)
#    include <glad/glad.h>
#endif

namespace Miren {

OpenGLVertexLayout::~OpenGLVertexLayout() {
    glDeleteVertexArrays(1, &m_id);
}

OpenGLVertexLayout::OpenGLVertexLayout(VertexBufferLayoutData *data) {
    glGenVertexArrays(1, &m_id);
    glBindVertexArray(m_id);
    long pointer = 0;
    for (int i = 0; i < data->m_elementsCount; i++) {
        glEnableVertexAttribArray(i);
        int type;
        switch (data->m_elements[i].type) {
            case BufferElementType::Float:
                type = GL_FLOAT;
                break;
            case BufferElementType::UnsignedInt:
                type = GL_UNSIGNED_INT;
                break;
            case BufferElementType::UnsignedByte:
                type = GL_UNSIGNED_BYTE;
                break;
            default:
                ASSERT(false, "Buffer element type is undefined");
                break;
        }
        glVertexAttribPointer(
            i, data->m_elements[i].count, type, data->m_elements[i].normalized ? GL_TRUE : GL_FALSE, data->m_stride, (const void *)pointer);
        pointer += data->m_elements[i].count * VertexBufferElement::getSizeOfType(data->m_elements[i].type);
    }
    glBindVertexArray(0);
}

void OpenGLVertexLayout::bind() {
    glBindVertexArray(m_id);
}

void OpenGLVertexLayout::unbind() {
    glBindVertexArray(0);
}

} // namespace Miren