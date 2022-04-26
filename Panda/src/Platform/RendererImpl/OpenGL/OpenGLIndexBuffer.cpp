//
// Created by Admin on 11.02.2022.
//

#include "OpenGLIndexBuffer.hpp"

#ifdef PND_PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PND_PLATFORM_DESKTOP)
#    include <glad/glad.h>
#endif

namespace Panda {

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    glDeleteBuffers(1, &id);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices, uint32_t count, bool isDynamic)
    : id(0) {
    this->isDynamic = isDynamic;
    this->size = count;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32_t), indices, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGLIndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void OpenGLIndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t OpenGLIndexBuffer::getSize() const {
    return size;
}

void OpenGLIndexBuffer::update(uint32_t *indices, uint32_t count) {
    if (isDynamic == false) {
        PND_ERROR("Невозможно обновить статичный буфер");
    }
    this->size = count;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32_t), indices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace Panda
