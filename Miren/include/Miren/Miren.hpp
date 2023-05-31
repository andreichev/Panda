//
// Created by Admin on 10.03.2022.
//

#pragma once

#include "Base.hpp"
#include "VertexBufferLayoutData.hpp"

namespace Miren {

void initialize();
void terminate();
// MARK: - Command buffer
FrameBufferHandle createFrameBuffer(FrameBufferSpecification specification);
void resizeFrameBuffer(FrameBufferHandle handle, uint32_t width, uint32_t height);
void deleteFrameBuffer(FrameBufferHandle handle);
ShaderHandle createShader(const char *vertexCode, const char *fragmentCode);
void deleteShader(ShaderHandle handle);
TextureHandle createTexture(TextureCreate create);
void deleteTexture(TextureHandle handle);
IndexBufferHandle createIndexBuffer(void *indices, BufferElementType elementType, size_t count);
IndexBufferHandle createDynamicIndexBuffer(
    void *indices, BufferElementType elementType, size_t count);
void updateDynamicIndexBuffer(IndexBufferHandle, void *indices, size_t count);
void deleteIndexBuffer(IndexBufferHandle handle);
// Only next frame vertex buffer
void allocTransientVertexBuffer(TransientVertexBuffer *buffer, uint32_t size);
// Only next frame index buffer
void allocTransientIndexBuffer(
    TransientIndexBuffer *buffer, uint32_t count, BufferElementType elementType);
VertexLayoutHandle createVertexLayout(VertexBufferLayoutData data);
VertexBufferHandle createVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle);
VertexBufferHandle createDynamicVertexBuffer(
    void *data, uint32_t size, VertexLayoutHandle layoutHandle = MIREN_INVALID_HANDLE);
void updateDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size);
void deleteVertexBuffer(VertexBufferHandle handle);
void deleteVertexLayout(VertexLayoutHandle handle);
// MARK: - Encoder setup
void setViewClear(ViewId id, uint32_t color);
void setViewport(ViewId id, Rect &rect);
void setState(uint32_t state);
void setScissorRect(Rect rect);
void setUniform(ShaderHandle handle, const char *name, void *value, UniformDataType type);
void setVertexBuffer(VertexBufferHandle handle, intptr_t offset = 0);
void setIndexBuffer(IndexBufferHandle handle, intptr_t offset, size_t count);
void setVertexLayout(VertexLayoutHandle handle);
void setShader(ShaderHandle handle);
void setTexture(TextureHandle textureHandle, uint32_t slot);
void setFrameBuffer(FrameBufferHandle frameBuffer);
/// Submit draw call
void submit(ViewId id);
// MARK: - Main functions
/// Process all requests to gpu (from rendering thread)
bool renderFrame();
/// Frame processing finished (from app thread). Wait for renderer to finish rendering frame.
/// Returns frame number
uint32_t frame();

void renderSemaphoreWait();
void renderSemaphorePost();

} // namespace Miren
