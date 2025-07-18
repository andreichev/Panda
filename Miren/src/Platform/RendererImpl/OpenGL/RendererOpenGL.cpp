//
// Created by Admin on 14.03.2022.
//

#include "RendererOpenGL.hpp"
#include "OpenGLBase.hpp"

#include <Foundation/Assert.hpp>
#include <Foundation/Allocator.hpp>
#include <Foundation/Logger.hpp>

namespace Miren {

void gpuErrorCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam
) {
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

RendererOpenGL::RendererOpenGL(Fern::GraphicsContext *ctx)
    : m_uselessVao(0)
    , m_frameBuffers()
    , m_shaders()
    , m_indexBuffers()
    , m_vertexLayouts()
    , m_vertexBuffers()
    , m_textures()
    , m_ctx(ctx) {
    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glDisable(GL_STENCIL_TEST));
    // GL_CALL(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    // glBlendEquation(GL_FUNC_ADD);
    // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    MIREN_LOG("OPENGL VERSION %s", (const char *)glGetString(GL_VERSION));
#if defined(PLATFORM_LINUX) || defined(PLATFORM_WINDOWS)
    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(gpuErrorCallback, nullptr);
#endif
    GL_CALL(glGenVertexArrays(1, &m_uselessVao));
    GL_CALL(glBindVertexArray(m_uselessVao));

    OpenGLExtensions::initialize();
}

RendererOpenGL::~RendererOpenGL() {
    GL_CALL(glDeleteVertexArrays(1, &m_uselessVao));
}

RendererType RendererOpenGL::getRendererType() const {
#ifdef PLATFORM_IOS
    return RendererType::OpenGLES;
#elif defined(PLATFORM_DESKTOP)
    return RendererType::OpenGL;
#endif
}

void RendererOpenGL::createFrameBuffer(
    FrameBufferHandle handle, FrameBufferSpecification specification
) {
    m_frameBuffers[handle.id].create(this, specification);
}

ProgramCreate RendererOpenGL::compileProgram(ProgramCompile compile) {
    // OpenGL has no shader compilation in version 3 which we support.
    // So we just return source code.
    return {compile.m_vertexCode, compile.m_fragmentCode};
}

void RendererOpenGL::readFrameBuffer(
    FrameBufferHandle handle, int attachIndex, int x, int y, int width, int height, void *data
) {
    m_frameBuffers[handle.id].readPixels(this, attachIndex, x, y, width, height, data);
}

void RendererOpenGL::deleteFrameBuffer(FrameBufferHandle handle) {
    m_frameBuffers[handle.id].terminate();
}

void RendererOpenGL::createProgram(ProgramHandle handle, ProgramCreate create) {
    m_shaders[handle.id].create(create);
}

void RendererOpenGL::deleteShader(ProgramHandle handle) {
    m_shaders[handle.id].terminate();
}

void RendererOpenGL::createTexture(TextureHandle handle, TextureCreate create) {
    m_textures[handle.id].create(create);
}

void RendererOpenGL::updateTexture(TextureHandle handle, Foundation::Memory mem) {
    m_textures[handle.id].update(mem);
}

void RendererOpenGL::resizeTexture(TextureHandle handle, uint32_t width, uint32_t height) {
    m_textures[handle.id].resize(width, height);
}

void RendererOpenGL::deleteTexture(TextureHandle handle) {
    m_textures[handle.id].terminate();
}

void RendererOpenGL::createIndexBuffer(
    IndexBufferHandle handle,
    Foundation::Memory indices,
    BufferElementType elementType,
    size_t count
) {
    m_indexBuffers[handle.id].create(indices.data, elementType, count, false);
    indices.release();
}

void RendererOpenGL::createDynamicIndexBuffer(
    IndexBufferHandle handle,
    Foundation::Memory indices,
    BufferElementType elementType,
    size_t count
) {
    m_indexBuffers[handle.id].create(indices.data, elementType, count, true);
    indices.release();
}

void RendererOpenGL::updateDynamicIndexBuffer(
    IndexBufferHandle handle, Foundation::Memory indices, size_t count
) {
    m_indexBuffers[handle.id].update(indices.data, count);
    indices.release();
}

void RendererOpenGL::deleteIndexBuffer(IndexBufferHandle handle) {
    m_indexBuffers[handle.id].terminate();
}

void RendererOpenGL::createVertexBuffer(
    VertexBufferHandle handle,
    Foundation::Memory data,
    uint32_t size,
    VertexLayoutHandle layoutHandle
) {
    m_vertexBuffers[handle.id].create(data.data, size, false);
    m_vertexBuffers[handle.id].setLayoutHandle(layoutHandle);
    data.release();
}

void RendererOpenGL::createDynamicVertexBuffer(
    VertexBufferHandle handle,
    Foundation::Memory data,
    uint32_t size,
    VertexLayoutHandle layoutHandle
) {
    m_vertexBuffers[handle.id].create(data.data, size, true);
    m_vertexBuffers[handle.id].setLayoutHandle(layoutHandle);
    data.release();
}

void RendererOpenGL::updateDynamicVertexBuffer(
    VertexBufferHandle handle, Foundation::Memory data, uint32_t size
) {
    m_vertexBuffers[handle.id].update(data.data, size);
    data.release();
}

void RendererOpenGL::deleteVertexBuffer(VertexBufferHandle handle) {
    m_vertexBuffers[handle.id].terminate();
}

void RendererOpenGL::createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) {
    m_vertexLayouts[handle.id] = layout;
}

void RendererOpenGL::deleteVertexLayout(VertexLayoutHandle handle) {}

void RendererOpenGL::readTexture(Miren::TextureHandle handle, void *data) {
    m_textures[handle.id].readPixels(data);
}

void RendererOpenGL::setUniform(const Uniform &uniform) {
    m_shaders[uniform.handle.id].bind();
    switch (uniform.type) {
        case UniformType::Sampler:
            m_shaders[uniform.handle.id].setUniformInt(
                uniform.name, static_cast<int *>(uniform.data), uniform.count
            );
            return;
        case UniformType::Mat3:
            m_shaders[uniform.handle.id].setUniformMat3(
                uniform.name, static_cast<float *>(uniform.data), uniform.count
            );
            return;
        case UniformType::Mat4:
            m_shaders[uniform.handle.id].setUniformMat4(
                uniform.name, static_cast<float *>(uniform.data), uniform.count
            );
            return;
        case UniformType::Float:
            m_shaders[uniform.handle.id].setUniformFloat(
                uniform.name, static_cast<float *>(uniform.data), uniform.count
            );
            return;
        case UniformType::Vec2:
            m_shaders[uniform.handle.id].setUniformVec2(
                uniform.name, static_cast<float *>(uniform.data), uniform.count
            );
            return;
        case UniformType::Vec3:
            m_shaders[uniform.handle.id].setUniformVec3(
                uniform.name, static_cast<float *>(uniform.data), uniform.count
            );
            return;
        case UniformType::Vec4:
            m_shaders[uniform.handle.id].setUniformVec4(
                uniform.name, static_cast<float *>(uniform.data), uniform.count
            );
            return;
        case UniformType::Count:
            return;
    }
    LOG_ERROR("UNIFORM TYPE IS UNDEFINED");
}

void RendererOpenGL::setTexture(TextureHandle handle, uint32_t slot) {
    m_textures[handle.id].bind(slot);
}

void RendererOpenGL::submit(Frame *frame) {
    MIREN_LOG("FRAME SUBMITTED. DRAW CALLS: %d", frame->m_drawCallsCount);
    if (frame->m_transientVbSize > 0) {
        m_vertexBuffers[frame->m_transientVb.handle.id].update(
            frame->m_transientVb.data, frame->m_transientVbSize
        );
    }
    if (frame->m_transientIbSize > 0) {
        m_indexBuffers[frame->m_transientIb.handle.id].update(
            frame->m_transientIb.data, frame->m_transientIbSize / 2
        );
    }
    if (!frame->m_drawCallsCount) {
        GL_CALL(glClearColor(0, 0, 0, 1));
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        return;
    }
    ViewId viewId = -1;
    for (int i = 0; i < frame->m_drawCallsCount; i++) {
        RenderDraw &draw = frame->m_drawCalls[i];
        if (!draw.m_isSubmitted) { continue; }
        if (draw.m_viewId != viewId) {
            viewId = draw.m_viewId;
            viewChanged(frame->m_views[viewId]);
        }
        submit(&draw);
    }
}

void RendererOpenGL::viewChanged(View &view) {
    if (view.m_frameBuffer.isValid()) {
        m_frameBuffers[view.m_frameBuffer.id].bind();
    } else {
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_ctx->getDefaultFrameBufferId()));
    }
    if (!view.m_viewport.isZero()) {
        GL_CALL(glViewport(
            view.m_viewport.origin.x,
            view.m_viewport.origin.y,
            view.m_viewport.size.width,
            view.m_viewport.size.height
        ));
    }
    uint32_t rgba = view.m_clearColor;
    uint8_t r = rgba >> 24;
    uint8_t g = rgba >> 16;
    uint8_t b = rgba >> 8;
    uint8_t a = rgba >> 0;
    GL_CALL(glClearColor((r) / 255.f, (g) / 255.f, (b) / 255.f, (a) / 255.f));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    if (!view.m_frameBuffer.isValid()) { return; }
    for (auto clear : view.m_clearAttachments) {
        // TODO: Add different clear types
        m_frameBuffers[view.m_frameBuffer.id].clearUIntAttachment(
            clear.attachmentIndex, clear.value
        );
    }
}

void RendererOpenGL::submit(RenderDraw *draw) {
    // TODO: Capture time
    if (!draw->m_shader.isValid()) { return; }
    m_shaders[draw->m_shader.id].bind();
    draw->m_uniformBuffer.finishWriting();
    Uniform *uniform;
    while ((uniform = draw->m_uniformBuffer.readUniform()) != nullptr) {
        setUniform(*uniform);
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
    if (draw->m_state & MIREN_STATE_WIREFRAME) {
        GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    } else {
        GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    }
    if (!draw->m_scissorRect.isZero()) {
        GL_CALL(glEnable(GL_SCISSOR_TEST));
        GL_CALL(glScissor(
            (int)draw->m_scissorRect.origin.x,
            (int)draw->m_scissorRect.origin.y,
            (int)draw->m_scissorRect.size.width,
            (int)draw->m_scissorRect.size.height
        ));
    } else {
        GL_CALL(glDisable(GL_SCISSOR_TEST));
    }
    m_vertexBuffers[draw->m_vertexBuffer.id].bind();
    VertexLayoutHandle layoutHandle =
        draw->m_vertexLayout.id != MIREN_INVALID_HANDLE
            ? draw->m_vertexLayout
            : m_vertexBuffers[draw->m_vertexBuffer.id].getLayoutHandle();
    PND_ASSERT(layoutHandle.id != MIREN_INVALID_HANDLE, "Invalid handle");
    VertexBufferLayoutData &layout = m_vertexLayouts[layoutHandle.id];
    GL_CALL(glBindVertexArray(m_uselessVao));
    m_shaders[draw->m_shader.id].bindAttributes(layout, draw->m_verticesOffset);
    m_indexBuffers[draw->m_indexBuffer.id].bind();
    GL_CALL(glDrawElements(
        GL_TRIANGLES,
        draw->m_numIndices,
        m_indexBuffers[draw->m_indexBuffer.id].getElementType(),
        (void *)draw->m_indicesOffset
    ));
    m_indexBuffers[draw->m_indexBuffer.id].unbind();
    m_shaders[draw->m_shader.id].unbind();
    m_vertexBuffers[draw->m_vertexBuffer.id].unbind();
    GL_CALL(glDisable(GL_SCISSOR_TEST));
    GL_CALL(glBindVertexArray(0));
}

} // namespace Miren
