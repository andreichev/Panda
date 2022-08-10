//
// Created by Admin on 14.03.2022.
//

#pragma once

#include "Panda/Renderer/RendererI.hpp"
#include "Panda/Renderer/GraphicsContext.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLIndexBuffer.hpp"
#include "OpenGLVertexBuffer.hpp"

namespace Panda {

class RendererOpenGL : public RendererI {
public:
    RendererOpenGL();
    ~RendererOpenGL() override;
    RendererType getRendererType() const override;
    void setViewportSize(UISize size) override;
    void setClearColor(float r, float g, float b, float a) override;
    void clear() override;
    void flip() override;
    void createShader(ShaderHandle handle, const char *vertexPath, const char *fragmentPath) override;
    void deleteShader(ShaderHandle handle) override;
    void createTextureFromFile(TextureHandle handle, const char *path) override;
    void createRGBATextureFromPixels(TextureHandle handle, void *pixels, int width, int height) override;
    void deleteTexture(TextureHandle handle) override;
    void createIndexBuffer(IndexBufferHandle handle, void *indices, BufferElementType elementType, size_t count) override;
    void createDynamicIndexBuffer(IndexBufferHandle handle, void *indices, BufferElementType elementType, size_t count) override;
    void updateDynamicIndexBuffer(IndexBufferHandle handle, void *indices, size_t count) override;
    void deleteIndexBuffer(IndexBufferHandle handle) override;
    void createVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size, VertexLayoutHandle layoutHandle) override;
    void createDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size, VertexLayoutHandle layoutHandle) override;
    void updateDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size) override;
    void deleteVertexBuffer(VertexBufferHandle handle) override;
    void createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) override;
    void deleteVertexLayout(VertexLayoutHandle handle) override;
    void setUniform(const Uniform &uniform) override;
    void setTexture(TextureHandle handle, uint32_t slot) override;
    void submit(RenderDraw* draw) override;

private:
    void checkForErrors();

    GraphicsContext *context;
    OpenGLShader *shaders[1000];
    OpenGLIndexBuffer *indexBuffers[1000];
    VertexBufferLayoutData *vertexLayouts[1000];
    OpenGLVertexBuffer *vertexBuffers[1000];
    OpenGLTexture *textures[1000];
};

} // namespace Panda
