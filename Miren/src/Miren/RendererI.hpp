//
// Created by Admin on 08.03.2022.
//

#pragma once

#include "Miren/Base.hpp"
#include "Miren/CommandBuffer/RenderDraw.hpp"
#include "Miren/CommandBuffer/Uniform.hpp"
#include "Miren/VertexBufferLayoutData.hpp"
#include "Miren/Vertex.hpp"

namespace Miren {

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
    virtual void setViewportSize(Size size) = 0;
    virtual void setClearColor(float r, float g, float b, float a) = 0;
    virtual void clear() = 0;
    virtual void flip() = 0;
    virtual void createShader(ShaderHandle handle, const char *vertexPath, const char *fragmentPath) = 0;
    virtual void deleteShader(ShaderHandle handle) = 0;
    virtual void createTextureFromFile(TextureHandle handle, const char *path) = 0;
    virtual void createRGBATextureFromPixels(TextureHandle handle, void *pixels, int width, int height) = 0;
    virtual void deleteTexture(TextureHandle handle) = 0;
    virtual void createIndexBuffer(IndexBufferHandle handle, void *indices, BufferElementType elementType, size_t count) = 0;
    virtual void createDynamicIndexBuffer(IndexBufferHandle handle, void *indices, BufferElementType elementType, size_t count) = 0;
    virtual void updateDynamicIndexBuffer(IndexBufferHandle handle, void *indices, size_t count) = 0;
    virtual void deleteIndexBuffer(IndexBufferHandle handle) = 0;
    virtual void createVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size, VertexLayoutHandle layoutHandle) = 0;
    virtual void createDynamicVertexBuffer(
        VertexBufferHandle handle, void *data, uint32_t size, VertexLayoutHandle layoutHandle = MIREN_INVALID_HANDLE) = 0;
    virtual void updateDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size) = 0;
    virtual void deleteVertexBuffer(VertexBufferHandle handle) = 0;
    virtual void createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) = 0;
    virtual void deleteVertexLayout(VertexLayoutHandle handle) = 0;
    virtual void setUniform(const Uniform &uniform) = 0;
    virtual void setTexture(TextureHandle handle, uint32_t slot) = 0;
    virtual void submit(RenderDraw *draw) = 0;
};

} // namespace Miren
