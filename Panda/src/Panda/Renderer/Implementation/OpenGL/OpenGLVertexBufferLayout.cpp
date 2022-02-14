//
// Created by Admin on 11.02.2022.
//
#include "pndpch.hpp"
#include "OpenGLVertexBufferLayout.hpp"

#include <glad/glad.h>

namespace Panda {

void OpenGLVertexBufferLayout::initializeForRenderer() {
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
    long pointer = 0;
    for (int i = 0; i < m_Elements.size(); i++) {
        glEnableVertexAttribArray(i);
        int type;
        switch (m_Elements[i].type) {
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
        glVertexAttribPointer(i, m_Elements[i].count, type, false, m_Stride, (const void *)pointer);
        pointer += m_Elements[i].count * VertexBufferElement::getSizeOfType(m_Elements[i].type);
    }
    glBindVertexArray(0);
}

void OpenGLVertexBufferLayout::bind() {
    glBindVertexArray(id);
}

void OpenGLVertexBufferLayout::unbind() {
    glBindVertexArray(0);
}

OpenGLVertexBufferLayout::~OpenGLVertexBufferLayout() {
    glDeleteVertexArrays(1, &id);
}

} // namespace Panda