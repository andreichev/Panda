//
// Created by Admin on 14.03.2022.
//

#include "RendererOpenGL.hpp"

#include <Foundation/Allocator.hpp>
#include <Foundation/Logger.hpp>
#include <Foundation/PlatformDetection.hpp>

#ifdef PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#    include "Platform/RendererImpl/Context/GlesContext.hpp"
#elif defined(PLATFORM_DESKTOP)
#    include <glad/glad.h>
#    include "Platform/RendererImpl/Context/OpenGLContext.hpp"
#endif

namespace Miren {

RendererOpenGL::RendererOpenGL() {
#ifdef PLATFORM_IOS
    context = new GlesContext();
#elif defined(PLATFORM_DESKTOP)
    context = new OpenGLContext();
#endif
    context->create();
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glEnable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendEquation(GL_FUNC_ADD);
    // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

RendererOpenGL::~RendererOpenGL() {
    delete context;
}

RendererType RendererOpenGL::getRendererType() const {
#ifdef PLATFORM_IOS
    return RendererType::OpenGLES;
#elif defined(PLATFORM_DESKTOP)
    return RendererType::OpenGL;
#endif
}

void RendererOpenGL::setViewportSize(Size size) {
    glViewport(0, 0, size.width, size.height);
}

void RendererOpenGL::setClearColor(float r, float g, float b, float a) {
    LOG_INFO("CLEAR COLOR: {}, {}, {}, {}", r, g, b, a);
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
    FREE(Foundation::getAllocator(), pixels);
}

void RendererOpenGL::deleteTexture(TextureHandle handle) {
    delete textures[handle];
    textures[handle] = nullptr;
}

void RendererOpenGL::createIndexBuffer(IndexBufferHandle handle, void *indices, BufferElementType elementType, size_t count) {
    indexBuffers[handle] = new OpenGLIndexBuffer(indices, elementType, count, false);
    FREE(Foundation::getAllocator(), indices);
}

void RendererOpenGL::createDynamicIndexBuffer(IndexBufferHandle handle, void *indices, BufferElementType elementType, size_t count) {
    indexBuffers[handle] = new OpenGLIndexBuffer(indices, elementType, count, true);
    FREE(Foundation::getAllocator(), indices);
}

void RendererOpenGL::updateDynamicIndexBuffer(IndexBufferHandle handle, void *indices, size_t count) {
    indexBuffers[handle]->update(indices, count);
    FREE(Foundation::getAllocator(), indices);
}

void RendererOpenGL::deleteIndexBuffer(IndexBufferHandle handle) {
    delete indexBuffers[handle];
    indexBuffers[handle] = nullptr;
}

void RendererOpenGL::createVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
    vertexBuffers[handle] = new OpenGLVertexBuffer(data, size, false, vertexLayouts[layoutHandle]);
    FREE(Foundation::getAllocator(), data);
}

void RendererOpenGL::createDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
    vertexBuffers[handle] = new OpenGLVertexBuffer(data, size, true, vertexLayouts[layoutHandle]);
    if (data != nullptr) {
        FREE(Foundation::getAllocator(), data);
    }
}

void RendererOpenGL::updateDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size) {
    vertexBuffers[handle]->update(data, size);
    FREE(Foundation::getAllocator(), data);
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
    LOG_ERROR("UIFORM TYPE IS UNDEFINED");
}

void RendererOpenGL::setTexture(TextureHandle handle, uint32_t slot) {
    textures[handle]->bind(slot);
}

void RendererOpenGL::submit(RenderDraw *draw) {
    if (vertexBuffers[draw->m_vertexBuffer] == nullptr) {
        return;
    }
    // TODO: Capture time
    if (draw->m_state & MIREN_STATE_CULL_FACE) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }
    if (draw->m_state & MIREN_STATE_DEPTH_TEST) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    if (draw->m_scissorRect.isZero() == false) {
        glEnable(GL_SCISSOR_TEST);
        glScissor((int)draw->m_scissorRect.origin.x, (int)draw->m_scissorRect.origin.y, (int)draw->m_scissorRect.size.width,
            (int)draw->m_scissorRect.size.height);
    } else {
        glDisable(GL_SCISSOR_TEST);
    }
    shaders[draw->m_shader]->bind();
    vertexBuffers[draw->m_vertexBuffer]->bind();
    if (draw->m_isIndexed) {
        indexBuffers[draw->m_indexBuffer]->bind();
        glDrawElements(GL_TRIANGLES, draw->m_numIndices, indexBuffers[draw->m_indexBuffer]->getElementType(), draw->m_indicesOffset);
        indexBuffers[draw->m_indexBuffer]->unbind();
    } else {
        // TODO: Add offset value
        glDrawArrays(GL_TRIANGLES, 0, draw->m_numElemets);
    }
    checkForErrors();
    shaders[draw->m_shader]->unbind();
    vertexBuffers[draw->m_vertexBuffer]->unbind();
    glDisable(GL_SCISSOR_TEST);
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
        LOG_ERROR("OPENGL: {}", getGLErrorStr(err));
    }
}

} // namespace Miren
