//
// Created by Admin on 08.03.2022.
//

#pragma once

#include "Panda/Base/GSize.hpp"
#include "VertexBufferLayoutData.hpp"
#include "Vertex.hpp"

namespace Panda {

using ShaderHandle = int;
using TextureHandle = int;
using IndexBufferHandle = int;
using VertexBufferHandle = int;
using VertexLayoutHandle = int;

enum class RendererType {
    Noop, //!< No rendering.
    // Direct3D12,   //!< Direct3D 12.0
    // Metal,        //!< Metal
    OpenGLES, //!< OpenGL ES 2.0+
    OpenGL,   //!< OpenGL 2.1+
    // Vulkan,       //!< Vulkan
};

class RendererI {
public:
    virtual ~RendererI() = default;
    virtual RendererType getRendererType() const = 0;
    virtual void setViewportSize(GSize size) = 0;
    virtual void setClearColor(float r, float g, float b, float a) = 0;
    virtual void clear() = 0;
    virtual void flip() = 0;
    virtual void semaphoreWait() = 0;
    virtual void semaphoreSignal() = 0;
    virtual void createShader(ShaderHandle handle, const char *vertexPath, const char *fragmentPath) = 0;
    virtual void createTexture(TextureHandle handle, const char *path) = 0;
    virtual void createIndexBuffer(IndexBufferHandle handle, uint32_t *indices, uint32_t count, bool isDynamic) = 0;
    virtual void createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) = 0;
    virtual void setUniform(ShaderHandle handle, const char *name, void *value, uint16_t size) = 0;
    virtual void createVertexBuffer(
        VertexBufferHandle handle, float *data, uint32_t count, bool isDynamic, VertexLayoutHandle layoutHandle) = 0;
    virtual void setTexture(TextureHandle handle, uint32_t slot) = 0;
    virtual void submit(ShaderHandle shader, VertexBufferHandle vertexBuffer, IndexBufferHandle indexBuffer, uint32_t indicesCount) = 0;
};

} // namespace Panda
