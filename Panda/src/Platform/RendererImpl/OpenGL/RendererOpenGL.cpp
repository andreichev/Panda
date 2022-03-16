//
// Created by Admin on 14.03.2022.
//
#include "pndpch.hpp"
#include "RendererOpenGL.hpp"

#include "Panda/Application/ApplicationContext.hpp"

#ifdef PND_PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#    include "Platform/RendererImpl/Context/GlesContext.hpp"
#elif defined(PND_PLATFORM_DESKTOP)
#    include <glad/glad.h>
#    include "Platform/RendererImpl/Context/OpenGLContext.hpp"
#endif

namespace Panda {

RendererOpenGL::RendererOpenGL(GSize size) {
#ifdef PND_PLATFORM_IOS
    context = new GlesContext();
#elif defined(PND_PLATFORM_DESKTOP)
    context = new OpenGLContext();
#endif
    context->create(size.width, size.height);
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

RendererOpenGL::~RendererOpenGL() {
    for(auto ref : shaders) {
        if(ref != nullptr) {
            delete ref;
        }
    }
    for(auto ref : indexBuffers) {
        if(ref != nullptr) {
            delete ref;
        }
    }
    for(auto ref : vertexLayouts) {
        if(ref != nullptr) {
            delete ref;
        }
    }
    for(auto ref : vertexBuffers) {
        if(ref != nullptr) {
            delete ref;
        }
    }
    for(auto ref : textures) {
        if(ref != nullptr) {
            delete ref;
        }
    }
    delete context;
}

RendererType RendererOpenGL::getRendererType() const {
    return RendererType::OpenGL;
}

void RendererOpenGL::semaphoreWait() {
    context->semaphoreWait();
}

void RendererOpenGL::semaphoreSignal() {
    context->semaphoreSignal();
}

void RendererOpenGL::createShader(ShaderHandle handle, const char *vertexPath, const char *fragmentPath) {
    shaders[handle] = new OpenGLShader(vertexPath, fragmentPath);
}

void RendererOpenGL::createTexture(TextureHandle handle, const char *path) {
    textures[handle] = new OpenGLTexture(path);
}

void RendererOpenGL::createIndexBuffer(IndexBufferHandle handle, uint32_t *indices, uint32_t count, bool isDynamic) {
    indexBuffers[handle] = new OpenGLIndexBuffer(indices, count, isDynamic);
}

void RendererOpenGL::createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) {
    vertexLayouts[handle] = new VertexBufferLayoutData(layout);
}

void RendererOpenGL::setUniform(ShaderHandle handle, const char *name, void *value, uint16_t size) {
    shaders[handle]->use();
    shaders[handle]->setUniformMat4(name, static_cast<float *>(value));
}

void RendererOpenGL::createVertexBuffer(
    VertexBufferHandle handle, float *data, uint32_t count, bool isDynamic, VertexLayoutHandle layoutHandle) {
    vertexBuffers[handle] = new OpenGLVertexBuffer(data, count, isDynamic, vertexLayouts[layoutHandle]);
}

void RendererOpenGL::setTexture(TextureHandle handle, uint32_t slot) {
    textures[handle]->bind(slot);
}

void RendererOpenGL::submit(ShaderHandle shader, VertexBufferHandle vertexBuffer, IndexBufferHandle indexBuffer, uint32_t indicesCount) {
    if (vertexBuffers[vertexBuffer] == nullptr) { return; }
    shaders[shader]->use();
    vertexBuffers[vertexBuffer]->bind();
    indexBuffers[indexBuffer]->bind();
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
    checkForErrors();
}

void RendererOpenGL::setViewportSize(GSize size) {
    glViewport(0, 0, (int)size.width, (int)size.height);
}

void RendererOpenGL::setClearColor(float r, float g, float b, float a) {
    PND_INFO("CLEAR COLOR: {}, {}, {}, {}", r, g, b, a);
    glClearColor(r, g, b, a);
}

void RendererOpenGL::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererOpenGL::flip() {
    context->flip();
}

const char *getGLErrorStr(GLenum err) {
    switch (err) {
        case GL_NO_ERROR:
            return "No error";
        case GL_INVALID_ENUM:
            return "Invalid enum";
        case GL_INVALID_VALUE:
            return "Invalid value";
        case GL_INVALID_OPERATION:
            return "Invalid operation";
            // case GL_STACK_OVERFLOW:    return "Stack overflow";
            // case GL_STACK_UNDERFLOW:   return "Stack underflow";
        case GL_OUT_OF_MEMORY:
            return "Out of memory";
        default:
            return "Unknown error";
    }
}

void RendererOpenGL::checkForErrors() {
    while (true) {
        const GLenum err = glGetError();
        if (GL_NO_ERROR == err)
            break;
        PND_ERROR("OPENGL: {}", getGLErrorStr(err));
    }
}

} // namespace Panda
