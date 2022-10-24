//
// Created by Admin on 10.03.2022.
//

#pragma once

#include "RendererI.hpp"
#include "CommandBuffer/Frame.hpp"
#include "CommandBuffer/RenderBuffers.hpp"
#include "CommandBuffer/RenderDraw.hpp"
#include "CommandBuffer/CommandQueue.hpp"
#include "Panda/Thread/Semaphore.hpp"
#include "MirenStates.hpp"

namespace Panda {

namespace Miren {

    void initialize();
    void terminate();
    // MARK: - Command buffer
    ShaderHandle createShader(const char *vertexPath, const char *fragmentPath);
    void deleteShader(ShaderHandle handle);
    TextureHandle createTextureFromFile(const char *path);
    TextureHandle createTextureFromPixels(void *pixels, int width, int height);
    void deleteTexture(TextureHandle handle);
    IndexBufferHandle createIndexBuffer(void *indices, BufferElementType elementType, size_t count);
    IndexBufferHandle createDynamicIndexBuffer(void *indices, BufferElementType elementType, size_t count);
    void updateDynamicIndexBuffer(IndexBufferHandle, void *indices, size_t count);
    void deleteIndexBuffer(IndexBufferHandle handle);
    // Only next frame vertex buffer
    void allocTransientVertexBuffer(TransientVertexBuffer *buffer, int32_t count, const VertexLayoutHandle layoutHandle);
    // Only next frame index buffer
    void allocTransientIndexBuffer(TransientIndexBuffer *buffer, uint32_t count, BufferElementType elementType);
    VertexBufferHandle createVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle);
    VertexBufferHandle createDynamicVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle);
    void updateDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size);
    void deleteVertexBuffer(VertexBufferHandle handle);
    VertexLayoutHandle createVertexLayout(VertexBufferLayoutData data);
    void deleteVertexLayout(VertexLayoutHandle handle);
    // MARK: - Encoder setup
    void setState(uint32_t state);
    void setScissorRect(UIRect rect);
    void setUniform(ShaderHandle handle, const char *name, void *value, UniformDataType type);
    void setVertexBuffer(VertexBufferHandle handle);
    void setIndexBuffer(IndexBufferHandle handle, void *offset, size_t count);
    void setShader(ShaderHandle handle);
    void setTexture(TextureHandle textureHandle, uint32_t slot);
    /// Submit draw call
    void submit();
    /// Submit not indexed triangles draw call
    void submitPrimitives(uint32_t elements);
    // MARK: - Main functions
    /// Process all requests to gpu (from rendering thread)
    void renderFrame();
    /// Frame processing finished (from app thread). Wait for renderer to finish rendering frame
    void frame();

    // TODO: - Remove
    void renderSemaphoreWait();
    void renderSemaphorePost();

}

} // namespace Panda
