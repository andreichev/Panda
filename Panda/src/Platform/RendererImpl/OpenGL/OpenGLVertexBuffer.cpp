//
// Created by Admin on 11.02.2022.
//

#include "OpenGLVertexBuffer.hpp"
#include "OpenGLVertexBufferLayout.hpp"

#ifdef PND_PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PND_PLATFORM_DESKTOP)
#    include <glad/glad.h>
#endif

namespace Panda {

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    delete layout;
    glDeleteBuffers(1, &id);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(Vertex *data, unsigned int count, bool isDynamic)
    : isDynamic(isDynamic)
    , id(0) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vertex), data, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    layout = new OpenGLVertexBufferLayout();
    layout->pushVector();
    layout->initializeForRenderer();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float *data, unsigned int count, bool isDynamic, VertexBufferLayout *layout)
    : isDynamic(isDynamic)
    , id(0) {
    this->layout = layout;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    layout->initializeForRenderer();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::update(float *data, unsigned int count) {
    if (isDynamic == false) {
        PND_CRITICAL("Невозможно обновить статичный буфер");
    }
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::update(Vertex *data, unsigned int count) {
    if (isDynamic == false) {
        PND_CRITICAL("Невозможно обновить статичный буфер");
    }
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vertex), data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void OpenGLVertexBuffer::bind() {
    layout->bind();
}
void OpenGLVertexBuffer::unbind() {
    layout->unbind();
}

} // namespace Panda
