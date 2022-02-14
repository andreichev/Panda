//
// Created by Admin on 11.02.2022.
//

#include "OpenGLIndexBuffer.hpp"

#include <glad/glad.h>

namespace Panda {

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    glDeleteBuffers(1, &id);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int *indices, unsigned int count, bool isDynamic)
    : id(0) {
    this->isDynamic = isDynamic;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    size = count;
}

void OpenGLIndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void OpenGLIndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int OpenGLIndexBuffer::getSize() const {
    return size;
}

void OpenGLIndexBuffer::update(unsigned int *indices) {
    if (isDynamic == false) {
        PND_ERROR("Невозможно обновить статичный буфер");
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace Panda