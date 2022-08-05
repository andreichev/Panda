//
// Created by Admin on 14.03.2022.
//
#include "pndpch.hpp"
#include "RendererOpenGL.hpp"

#include "Panda/Application/PlatformDetection.hpp"

#ifdef PND_PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#    include "Platform/RendererImpl/Context/GlesContext.hpp"
#elif defined(PND_PLATFORM_DESKTOP)
#    include <glad/glad.h>
#    include "Platform/RendererImpl/Context/OpenGLContext.hpp"
#endif

namespace Panda {

RendererOpenGL::RendererOpenGL() {
#ifdef PND_PLATFORM_IOS
    context = new GlesContext();
#elif defined(PND_PLATFORM_DESKTOP)
    context = new OpenGLContext();
#endif
    context->create();
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glEnable(GL_DEPTH_TEST);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    // glDisable(GL_CULL_FACE);
    // glDisable(GL_DEPTH_TEST);
    // glDisable(GL_STENCIL_TEST);
    // glEnable(GL_SCISSOR_TEST);
}

RendererOpenGL::~RendererOpenGL() {
    delete context;
}

RendererType RendererOpenGL::getRendererType() const {
#ifdef PND_PLATFORM_IOS
    return RendererType::OpenGLES;
#elif defined(PND_PLATFORM_DESKTOP)
    return RendererType::OpenGL;
#endif
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

void RendererOpenGL::createShader(ShaderHandle handle, const char *vertexPath, const char *fragmentPath) {
    shaders[handle] = new OpenGLShader(vertexPath, fragmentPath);
}

void RendererOpenGL::deleteShader(ShaderHandle handle) {
    delete shaders[handle];
    shaders[handle] = nullptr;
}

void RendererOpenGL::createTextureFromFile(TextureHandle handle, const char *path) {
    textures[handle] = new OpenGLTexture(path);
}

void RendererOpenGL::createRGBATextureFromPixels(TextureHandle handle, void *pixels, int width, int height) {
    textures[handle] = new OpenGLTexture(pixels, width, height);
    free(pixels);
}

void RendererOpenGL::deleteTexture(TextureHandle handle) {
    delete textures[handle];
    textures[handle] = nullptr;
}

void RendererOpenGL::createIndexBuffer(IndexBufferHandle handle, void *indices, BufferElementType elementType, size_t count) {
    indexBuffers[handle] = new OpenGLIndexBuffer(indices, elementType, count, false);
    free(indices);
}

void RendererOpenGL::createDynamicIndexBuffer(IndexBufferHandle handle, void *indices, BufferElementType elementType, size_t count) {
    indexBuffers[handle] = new OpenGLIndexBuffer(indices, elementType, count, true);
    free(indices);
}

void RendererOpenGL::updateDynamicIndexBuffer(IndexBufferHandle handle, void *indices, size_t count) {
    indexBuffers[handle]->update(indices, count);
    free(indices);
}

void RendererOpenGL::deleteIndexBuffer(IndexBufferHandle handle) {
    delete indexBuffers[handle];
    indexBuffers[handle] = nullptr;
}

void RendererOpenGL::createVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
    vertexBuffers[handle] = new OpenGLVertexBuffer(data, size, false, vertexLayouts[layoutHandle]);
    free(data);
}

void RendererOpenGL::createDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
    vertexBuffers[handle] = new OpenGLVertexBuffer(data, size, true, vertexLayouts[layoutHandle]);
    free(data);
}

void RendererOpenGL::updateDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size) {
    vertexBuffers[handle]->update(data, size);
    free(data);
}

void RendererOpenGL::deleteVertexBuffer(VertexBufferHandle handle) {
    delete vertexBuffers[handle];
    vertexBuffers[handle] = nullptr;
}

void RendererOpenGL::createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) {
    vertexLayouts[handle] = new VertexBufferLayoutData(layout);
}

void RendererOpenGL::deleteVertexLayout(VertexLayoutHandle handle) {
    delete vertexLayouts[handle];
    vertexLayouts[handle] = nullptr;
}

void RendererOpenGL::setUniform(const Uniform &uniform) {
    shaders[uniform.handle]->bind();
    switch (uniform.type) {
        case UniformDataType::Mat4:
            shaders[uniform.handle]->setUniformMat4(uniform.name, static_cast<float *>(uniform.value));
            return;
        case UniformDataType::Int:
            shaders[uniform.handle]->setUniformInt(uniform.name, (int)(intptr_t)uniform.value);
            return;
    }
    PND_ERROR("UIFORM TYPE IS UNDEFINED");
}

void RendererOpenGL::setTexture(TextureHandle handle, uint32_t slot) {
    textures[handle]->bind(slot);
}

void RendererOpenGL::submitIndexed(
    ShaderHandle shader, VertexBufferHandle vertexBuffer, IndexBufferHandle indexBuffer, void *offset, uint32_t indicesCount) {
    if (vertexBuffers[vertexBuffer] == nullptr) {
        return;
    }
    shaders[shader]->bind();
    vertexBuffers[vertexBuffer]->bind();
    indexBuffers[indexBuffer]->bind();
    // TODO: Capture time
    glDrawElements(GL_TRIANGLES, indicesCount, indexBuffers[indexBuffer]->getElementType(), offset);
    checkForErrors();
    shaders[shader]->unbind();
    indexBuffers[indexBuffer]->unbind();
    vertexBuffers[vertexBuffer]->unbind();
}

void RendererOpenGL::submitPrimitives(ShaderHandle shader, VertexBufferHandle vertexBuffer, uint32_t elementsCount) {
    if (vertexBuffers[vertexBuffer] == nullptr) {
        return;
    }
    shaders[shader]->bind();
    vertexBuffers[vertexBuffer]->bind();
    // TODO: Capture time
    glDrawArrays(GL_TRIANGLES, 0, elementsCount);
    checkForErrors();
    shaders[shader]->unbind();
    vertexBuffers[vertexBuffer]->unbind();
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
