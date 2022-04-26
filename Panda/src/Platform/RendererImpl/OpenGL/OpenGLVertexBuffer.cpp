//
// Created by Admin on 11.02.2022.
//

#include "OpenGLVertexBuffer.hpp"

#ifdef PND_PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PND_PLATFORM_DESKTOP)
#    include <glad/glad.h>
#endif

namespace Panda {

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    glDeleteVertexArrays(1, &layoutId);
    glDeleteBuffers(1, &id);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(void *data, uint32_t size, bool isDynamic, VertexBufferLayoutData *layout)
    : isDynamic(isDynamic)
    , id(0) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    createLayout(layout);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::update(void *data, uint32_t size) {
    if (isDynamic == false) {
        PND_CRITICAL("Невозможно обновить статичный буфер");
    }
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::bind() {
    glBindVertexArray(layoutId);
}
void OpenGLVertexBuffer::unbind() {
    glBindVertexArray(0);
}

void OpenGLVertexBuffer::createLayout(VertexBufferLayoutData *data) {
    glGenVertexArrays(1, &layoutId);
    glBindVertexArray(layoutId);
    long pointer = 0;
    for (int i = 0; i < data->m_Elements.size(); i++) {
        glEnableVertexAttribArray(i);
        int type;
        switch (data->m_Elements[i].type) {
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
                PND_ERROR("Buffer element type is undefined");
                break;
        }
        glVertexAttribPointer(i, data->m_Elements[i].count, type, false, data->m_Stride, (const void *)pointer);
        pointer += data->m_Elements[i].count * VertexBufferElement::getSizeOfType(data->m_Elements[i].type);
    }
    glBindVertexArray(0);
}

} // namespace Panda
