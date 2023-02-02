//
// Created by Admin on 14.03.2022.
//

#include "RendererOpenGL.hpp"

#include <Foundation/Assert.hpp>
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
    context = NEW(Foundation::getAllocator(), GlesContext);
#elif defined(PLATFORM_DESKTOP)
    context = NEW(Foundation::getAllocator(), OpenGLContext);
#endif
    context->create();
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glEnable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendEquation(GL_FUNC_ADD);
    // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
}

RendererOpenGL::~RendererOpenGL() {
    glDeleteVertexArrays(1, &m_vao);
    DELETE(Foundation::getAllocator(), context);
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
    shaders[handle].create(vertexPath, fragmentPath);
}

void RendererOpenGL::deleteShader(ShaderHandle handle) {
    shaders[handle].terminate();
}

void RendererOpenGL::createTextureFromFile(TextureHandle handle, const char *path) {
    textures[handle].create(path);
}

void RendererOpenGL::createRGBATextureFromPixels(TextureHandle handle, void *pixels, int width, int height) {
    textures[handle].create(pixels, width, height);
    FREE(Foundation::getAllocator(), pixels);
}

void RendererOpenGL::deleteTexture(TextureHandle handle) {
    textures[handle].terminate();
}

void RendererOpenGL::createIndexBuffer(IndexBufferHandle handle, void *indices, BufferElementType elementType, size_t count) {
    indexBuffers[handle].create(indices, elementType, count, false);
    FREE(Foundation::getAllocator(), indices);
}

void RendererOpenGL::createDynamicIndexBuffer(IndexBufferHandle handle, void *indices, BufferElementType elementType, size_t count) {
    indexBuffers[handle].create(indices, elementType, count, true);
    FREE(Foundation::getAllocator(), indices);
}

void RendererOpenGL::updateDynamicIndexBuffer(IndexBufferHandle handle, void *indices, size_t count) {
    indexBuffers[handle].update(indices, count);
    if (indices != nullptr) {
        FREE(Foundation::getAllocator(), indices);
    }
}

void RendererOpenGL::deleteIndexBuffer(IndexBufferHandle handle) {
    indexBuffers[handle].terminate();
}

void RendererOpenGL::createVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
    vertexBuffers[handle].create(data, size, false);
    vertexBuffers[handle].setLayoutHandle(layoutHandle);
    FREE(Foundation::getAllocator(), data);
}

void RendererOpenGL::createDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
    vertexBuffers[handle].create(data, size, true);
    vertexBuffers[handle].setLayoutHandle(layoutHandle);
    if (data != nullptr) {
        FREE(Foundation::getAllocator(), data);
    }
}

void RendererOpenGL::updateDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size) {
    vertexBuffers[handle].update(data, size);
    FREE(Foundation::getAllocator(), data);
}

void RendererOpenGL::deleteVertexBuffer(VertexBufferHandle handle) {
    vertexBuffers[handle].terminate();
}

void RendererOpenGL::createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) {
    vertexLayouts[handle] = layout;
}

void RendererOpenGL::deleteVertexLayout(VertexLayoutHandle handle) {}

void RendererOpenGL::setUniform(const Uniform &uniform) {
    shaders[uniform.handle].bind();
    switch (uniform.type) {
        case UniformDataType::Mat4:
            shaders[uniform.handle].setUniformMat4(uniform.name, static_cast<float *>(uniform.value));
            return;
        case UniformDataType::Int:
            shaders[uniform.handle].setUniformInt(uniform.name, (int)(intptr_t)uniform.value);
            return;
    }
    LOG_ERROR("UIFORM TYPE IS UNDEFINED");
}

void RendererOpenGL::setTexture(TextureHandle handle, uint32_t slot) {
    textures[handle].bind(slot);
}

void RendererOpenGL::submit(Frame *frame) {
    if (frame->m_viewport.isZero() == false) {
        glViewport(frame->m_viewport.origin.x, frame->m_viewport.origin.y, frame->m_viewport.size.width, frame->m_viewport.size.height);
    }
    if (frame->m_transientVbSize > 0) {
        vertexBuffers[frame->m_transientVb.handle].update(frame->m_transientVb.data, frame->m_transientVbSize);
    }
    if (frame->m_transientIbSize > 0) {
        indexBuffers[frame->m_transientIb.handle].update(frame->m_transientIb.data, frame->m_transientIbSize / 2);
    }
    for (int i = 0; i < frame->getDrawCallsCount(); i++) {
        RenderDraw &draw = frame->getDrawCalls()[i];
        if (draw.m_isSubmitted == false) {
            continue;
        }
        for (size_t u = 0; u < draw.m_uniformsCount; u++) {
            Uniform &uniform = draw.m_uniformBuffer[u];
            setUniform(uniform);
        }
        for (size_t t = 0; t < draw.m_textureBindingsCount; t++) {
            TextureBinding &textureBinding = draw.m_textureBindings[t];
            setTexture(textureBinding.m_handle, textureBinding.m_slot);
        }
        submit(&draw);
    }
}

void RendererOpenGL::submit(RenderDraw *draw) {
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
    vertexBuffers[draw->m_vertexBuffer].bind();
    VertexLayoutHandle layoutHandle =
        draw->m_vertexLayout != MIREN_INVALID_HANDLE ? draw->m_vertexLayout : vertexBuffers[draw->m_vertexBuffer].getLayoutHandle();
    PND_ASSERT(layoutHandle != MIREN_INVALID_HANDLE, "Invalid handle");
    VertexBufferLayoutData &layout = vertexLayouts[layoutHandle];
    glBindVertexArray(m_vao);
    shaders[draw->m_shader].bind();
    shaders[draw->m_shader].bindAttributes(layout, draw->m_verticesOffset);
    if (draw->m_isIndexed) {
        indexBuffers[draw->m_indexBuffer].bind();
        glDrawElements(GL_TRIANGLES, draw->m_numIndices, indexBuffers[draw->m_indexBuffer].getElementType(), (void *)draw->m_indicesOffset);
        indexBuffers[draw->m_indexBuffer].unbind();
    } else {
        // TODO: Add offset value
        glDrawArrays(GL_TRIANGLES, 0, draw->m_numElemets);
    }
    checkForErrors();
    shaders[draw->m_shader].unbind();
    vertexBuffers[draw->m_vertexBuffer].unbind();
    glDisable(GL_SCISSOR_TEST);
    glBindVertexArray(0);
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
        PND_ASSERT(false, "OPENGL ERROR");
    }
}

} // namespace Miren
