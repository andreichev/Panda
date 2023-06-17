//
// Created by Admin on 14.03.2022.
//

#include "RendererOpenGL.hpp"
#include "OpenGLBase.hpp"

#include <Foundation/Assert.hpp>
#include <Foundation/Allocator.hpp>
#include <Foundation/Logger.hpp>

#ifdef PLATFORM_IOS
#    include "Platform/RendererImpl/Context/GlesContext.hpp"
#elif defined(PLATFORM_DESKTOP)
#    include "Platform/RendererImpl/Context/OpenGLContext.hpp"
#endif

namespace Miren {

RendererOpenGL *RendererOpenGL::s_instance;

void gpuErrorCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam) {
    LOG_INFO(message);
    LOG_CRITICAL("OPENGL ERROR");
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

RendererOpenGL::RendererOpenGL() {
    s_instance = this;
#ifdef PLATFORM_IOS
    context = NEW(Foundation::getAllocator(), GlesContext);
#elif defined(PLATFORM_DESKTOP)
    context = NEW(Foundation::getAllocator(), OpenGLContext);
#endif
    context->create();
    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glDisable(GL_STENCIL_TEST));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    // glBlendEquation(GL_FUNC_ADD);
    // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    LOG_INFO("OPENGL VERSION {}", glGetString(GL_VERSION));
    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(gpuErrorCallback, nullptr);
    GL_CALL(glGenVertexArrays(1, &m_vao));
    GL_CALL(glBindVertexArray(m_vao));
}

RendererOpenGL::~RendererOpenGL() {
    GL_CALL(glDeleteVertexArrays(1, &m_vao));
    DELETE(Foundation::getAllocator(), context);
    s_instance = nullptr;
}

RendererType RendererOpenGL::getRendererType() const {
#ifdef PLATFORM_IOS
    return RendererType::OpenGLES;
#elif defined(PLATFORM_DESKTOP)
    return RendererType::OpenGL;
#endif
}

void RendererOpenGL::flip() {
    context->flip();
}

void RendererOpenGL::createFrameBuffer(
    FrameBufferHandle handle, FrameBufferSpecification specification) {
    frameBuffers[handle.id].create(specification);
}

void RendererOpenGL::deleteFrameBuffer(FrameBufferHandle handle) {
    frameBuffers[handle.id].terminate();
}

void RendererOpenGL::createShader(
    ShaderHandle handle, const char *vertexCode, const char *fragmentCode) {
    shaders[handle.id].create(vertexCode, fragmentCode);
}

void RendererOpenGL::deleteShader(ShaderHandle handle) {
    shaders[handle.id].terminate();
}

void RendererOpenGL::createTexture(TextureHandle handle, const TextureCreate &create) {
    textures[handle.id].create(create);
}

void RendererOpenGL::resizeTexture(TextureHandle handle, uint32_t width, uint32_t height) {
    textures[handle.id].resize(width, height);
}

void RendererOpenGL::deleteTexture(TextureHandle handle) {
    textures[handle.id].terminate();
}

void RendererOpenGL::createIndexBuffer(
    IndexBufferHandle handle, void *indices, BufferElementType elementType, size_t count) {
    indexBuffers[handle.id].create(indices, elementType, count, false);
    FREE(Foundation::getAllocator(), indices);
}

void RendererOpenGL::createDynamicIndexBuffer(
    IndexBufferHandle handle, void *indices, BufferElementType elementType, size_t count) {
    indexBuffers[handle.id].create(indices, elementType, count, true);
    if (indices != nullptr) {
        FREE(Foundation::getAllocator(), indices);
    }
}

void RendererOpenGL::updateDynamicIndexBuffer(
    IndexBufferHandle handle, void *indices, size_t count) {
    indexBuffers[handle.id].update(indices, count);
    if (indices != nullptr) {
        FREE(Foundation::getAllocator(), indices);
    }
}

void RendererOpenGL::deleteIndexBuffer(IndexBufferHandle handle) {
    indexBuffers[handle.id].terminate();
}

void RendererOpenGL::createVertexBuffer(
    VertexBufferHandle handle, void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
    vertexBuffers[handle.id].create(data, size, false);
    vertexBuffers[handle.id].setLayoutHandle(layoutHandle);
    FREE(Foundation::getAllocator(), data);
}

void RendererOpenGL::createDynamicVertexBuffer(
    VertexBufferHandle handle, void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
    vertexBuffers[handle.id].create(data, size, true);
    vertexBuffers[handle.id].setLayoutHandle(layoutHandle);
    if (data != nullptr) {
        FREE(Foundation::getAllocator(), data);
    }
}

void RendererOpenGL::updateDynamicVertexBuffer(
    VertexBufferHandle handle, void *data, uint32_t size) {
    vertexBuffers[handle.id].update(data, size);
    if (data != nullptr) {
        FREE(Foundation::getAllocator(), data);
    }
}

void RendererOpenGL::deleteVertexBuffer(VertexBufferHandle handle) {
    vertexBuffers[handle.id].terminate();
}

void RendererOpenGL::createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) {
    vertexLayouts[handle.id] = layout;
}

void RendererOpenGL::deleteVertexLayout(VertexLayoutHandle handle) {}

void RendererOpenGL::setUniform(const Uniform &uniform) {
    shaders[uniform.handle.id].bind();
    switch (uniform.type) {
        case UniformDataType::Mat4:
            shaders[uniform.handle.id].setUniformMat4(
                uniform.name, static_cast<float *>(uniform.value));
            return;
        case UniformDataType::Int:
            shaders[uniform.handle.id].setUniformInt(uniform.name, (int)(intptr_t)uniform.value);
            return;
        case UniformDataType::IntArray:
            shaders[uniform.handle.id].setUniformIntArray(uniform.name, (int *)uniform.value);
            return;
    }
    LOG_ERROR("UIFORM TYPE IS UNDEFINED");
}

void RendererOpenGL::setTexture(TextureHandle handle, uint32_t slot) {
    textures[handle.id].bind(slot);
}

void RendererOpenGL::submit(Frame *frame, View *views) {
    if (frame->m_transientVbSize > 0) {
        vertexBuffers[frame->m_transientVb.handle.id].update(
            frame->m_transientVb.data, frame->m_transientVbSize);
    }
    if (frame->m_transientIbSize > 0) {
        indexBuffers[frame->m_transientIb.handle.id].update(
            frame->m_transientIb.data, frame->m_transientIbSize / 2);
    }
    ViewId viewId = -1;
    for (int i = 0; i < frame->getDrawCallsCount(); i++) {
        RenderDraw &draw = frame->getDrawCalls()[i];
        if (draw.m_isSubmitted == false) {
            continue;
        }
        if (draw.m_viewId != viewId) {
            viewId = draw.m_viewId;
            viewChanged(views[viewId]);
        }
        submit(&draw);
    }
}

void RendererOpenGL::viewChanged(View &view) {
    if (view.m_frameBuffer.id != MIREN_INVALID_HANDLE) {
        frameBuffers[view.m_frameBuffer.id].bind();
    } else {
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
    if (view.m_viewport.isZero() == false) {
        GL_CALL(glViewport(view.m_viewport.origin.x,
            view.m_viewport.origin.y,
            view.m_viewport.size.width,
            view.m_viewport.size.height));
    }
    uint32_t rgba = view.m_clearColor;
    uint8_t r = rgba >> 24;
    uint8_t g = rgba >> 16;
    uint8_t b = rgba >> 8;
    uint8_t a = rgba >> 0;
    GL_CALL(glClearColor((r) / 255.f, (g) / 255.f, (b) / 255.f, (a) / 255.f));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void RendererOpenGL::submit(RenderDraw *draw) {
    // TODO: Capture time
    shaders[draw->m_shader.id].bind();
    for (size_t u = 0; u < draw->m_uniformsCount; u++) {
        Uniform &uniform = draw->m_uniformBuffer[u];
        setUniform(uniform);
    }
    for (size_t t = 0; t < draw->m_textureBindingsCount; t++) {
        TextureBinding &textureBinding = draw->m_textureBindings[t];
        setTexture(textureBinding.m_handle, textureBinding.m_slot);
    }
    if (draw->m_state & MIREN_STATE_CULL_FACE) {
        GL_CALL(glEnable(GL_CULL_FACE));
    } else {
        GL_CALL(glDisable(GL_CULL_FACE));
    }
    if (draw->m_state & MIREN_STATE_DEPTH_TEST) {
        GL_CALL(glEnable(GL_DEPTH_TEST));
    } else {
        GL_CALL(glDisable(GL_DEPTH_TEST));
    }
    if (draw->m_scissorRect.isZero() == false) {
        GL_CALL(glEnable(GL_SCISSOR_TEST));
        GL_CALL(glScissor((int)draw->m_scissorRect.origin.x,
            (int)draw->m_scissorRect.origin.y,
            (int)draw->m_scissorRect.size.width,
            (int)draw->m_scissorRect.size.height));
    } else {
        GL_CALL(glDisable(GL_SCISSOR_TEST));
    }
    vertexBuffers[draw->m_vertexBuffer.id].bind();
    VertexLayoutHandle layoutHandle =
        draw->m_vertexLayout.id != MIREN_INVALID_HANDLE
            ? draw->m_vertexLayout
            : vertexBuffers[draw->m_vertexBuffer.id].getLayoutHandle();
    PND_ASSERT(layoutHandle.id != MIREN_INVALID_HANDLE, "Invalid handle");
    VertexBufferLayoutData &layout = vertexLayouts[layoutHandle.id];
    GL_CALL(glBindVertexArray(m_vao));
    shaders[draw->m_shader.id].bindAttributes(layout, draw->m_verticesOffset);
    indexBuffers[draw->m_indexBuffer.id].bind();
    GL_CALL(glDrawElements(GL_TRIANGLES,
        draw->m_numIndices,
        indexBuffers[draw->m_indexBuffer.id].getElementType(),
        (void *)draw->m_indicesOffset));
    indexBuffers[draw->m_indexBuffer.id].unbind();
    shaders[draw->m_shader.id].unbind();
    vertexBuffers[draw->m_vertexBuffer.id].unbind();
    GL_CALL(glDisable(GL_SCISSOR_TEST));
    GL_CALL(glBindVertexArray(0));
}

} // namespace Miren
