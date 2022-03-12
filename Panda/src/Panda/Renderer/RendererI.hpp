//
// Created by Admin on 08.03.2022.
//

#pragma once

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
    virtual ~RendererI() = 0;
    virtual RendererType getRendererType() const = 0;
    virtual void semaphoreWait() = 0;
    virtual void semaphoreSignal() = 0;
    virtual void createShader(ShaderHandle handle, const char *vertexPath, const char *fragmentPath) = 0;
    virtual void createTexture(TextureHandle handle, const char *path) = 0;
    virtual void createIndexBuffer(IndexBufferHandle handle, unsigned int *indices, unsigned int count, bool isDynamic) = 0;
    virtual void createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) = 0;
    virtual void createVertexBuffer(
        VertexBufferHandle handle,
        float *data, unsigned int count, bool isDynamic, VertexLayoutHandle layoutHandle
    ) = 0;
};

}
