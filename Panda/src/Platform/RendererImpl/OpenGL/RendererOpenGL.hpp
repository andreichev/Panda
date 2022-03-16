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
    RendererOpenGL(GSize size);
    ~RendererOpenGL() override;
    RendererType getRendererType() const override;
    void semaphoreWait() override;
    void semaphoreSignal() override;
    void setViewportSize(GSize size) override;
    void setClearColor(float r, float g, float b, float a) override;
    void clear() override;
    void flip() override;
    void createShader(ShaderHandle handle, const char *vertexPath, const char *fragmentPath) override;
    void createTexture(TextureHandle handle, const char *path) override;
    void createIndexBuffer(IndexBufferHandle handle, uint32_t *indices, uint32_t count, bool isDynamic) override;
    void createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) override;
    void setUniform(ShaderHandle handle, const char *name, void *value, uint16_t size) override;
    void createVertexBuffer(
        VertexBufferHandle handle, float *data, uint32_t count, bool isDynamic, VertexLayoutHandle layoutHandle) override;
    void setTexture(TextureHandle handle, uint32_t slot) override;
    void submit(ShaderHandle shader, VertexBufferHandle vertexBuffer, IndexBufferHandle indexBuffer, uint32_t indicesCount) override;

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
