//
// Created by Admin on 11.02.2022.
//

#include "OpenGLIndexBuffer.hpp"

#include <Foundation/PlatformDetection.hpp>

#ifdef PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PLATFORM_DESKTOP)
#    include <glad/glad.h>
#endif

namespace Miren {

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    glDeleteBuffers(1, &id);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(void *indices, BufferElementType elementType, size_t count, bool isDynamic)
    : id(0) {
    this->isDynamic = isDynamic;
    this->count = count;

    if (elementType == BufferElementType::UnsignedByte) {
        this->elementType = GL_UNSIGNED_BYTE;
        this->elementSize = 1;
    } else if (elementType == BufferElementType::UnsignedShort) {
        this->elementType = GL_UNSIGNED_SHORT;
        this->elementSize = 2;
    } else {
        this->elementType = GL_UNSIGNED_INT;
        this->elementSize = 4;
    }
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * elementSize, indices, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGLIndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void OpenGLIndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t OpenGLIndexBuffer::getCount() const {
    return count;
}

void OpenGLIndexBuffer::update(void *indices, size_t count) {
    if (isDynamic == false) {
        LOG_ERROR("Невозможно обновить статичный буфер");
    }
    this->count = count;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * elementSize, indices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace Miren
