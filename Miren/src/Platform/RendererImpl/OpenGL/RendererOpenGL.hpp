//
// Created by Admin on 14.03.2022.
//

#pragma once

#include "Miren/RendererI.hpp"
#include "Miren/GraphicsContext.hpp"
#include "OpenGLFrameBuffer.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLIndexBuffer.hpp"
#include "OpenGLVertexBuffer.hpp"

namespace Miren {

class RendererOpenGL : public RendererI {
public:
    RendererOpenGL();
    ~RendererOpenGL() override;
    RendererType getRendererType() const override;
    void flip() override;
    void createFrameBuffer(
        FrameBufferHandle handle, FrameBufferSpecification specification) override;
    void resizeFrameBuffer(FrameBufferHandle handle, uint32_t width, uint32_t height) override;
    void deleteFrameBuffer(FrameBufferHandle handle) override;
    void createShader(
        ShaderHandle handle, const char *vertexCode, const char *fragmentCode) override;
    void deleteShader(ShaderHandle handle) override;
    void createTexture(TextureHandle handle, const TextureCreate &create) override;
    void deleteTexture(TextureHandle handle) override;
    void createIndexBuffer(IndexBufferHandle handle,
        void *indices,
        BufferElementType elementType,
        size_t count) override;
    void createDynamicIndexBuffer(IndexBufferHandle handle,
        void *indices,
        BufferElementType elementType,
        size_t count) override;
    void updateDynamicIndexBuffer(IndexBufferHandle handle, void *indices, size_t count) override;
    void deleteIndexBuffer(IndexBufferHandle handle) override;
    void createVertexBuffer(VertexBufferHandle handle,
        void *data,
        uint32_t size,
        VertexLayoutHandle layoutHandle) override;
    void createDynamicVertexBuffer(VertexBufferHandle handle,
        void *data,
        uint32_t size,
        VertexLayoutHandle layoutHandle) override;
    void updateDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size) override;
    void deleteVertexBuffer(VertexBufferHandle handle) override;
    void createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) override;
    void deleteVertexLayout(VertexLayoutHandle handle) override;
    void setUniform(const Uniform &uniform) override;
    void setTexture(TextureHandle handle, uint32_t slot) override;
    void submit(Frame *frame, View *views) override;

    static RendererOpenGL *s_instance;
    OpenGLTexture &getTexture(TextureHandle handle) {
        return textures[handle];
    }

private:
    void viewChanged(View &view);
    void submit(RenderDraw *draw);
    void checkForErrors();

    uint32_t m_vao;
    GraphicsContext *context;
    OpenGLFrameBuffer frameBuffers[1000];
    OpenGLShader shaders[1000];
    OpenGLIndexBuffer indexBuffers[1000];
    VertexBufferLayoutData vertexLayouts[1000];
    OpenGLVertexBuffer vertexBuffers[1000];
    OpenGLTexture textures[1000];
};

} // namespace Miren
