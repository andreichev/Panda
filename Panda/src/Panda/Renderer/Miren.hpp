//
// Created by Admin on 10.03.2022.
//

#pragma once

#include "MirenHandleAllocator.hpp"
#include "RendererI.hpp"
#include "CommandBuffer/Frame.hpp"
#include "CommandBuffer/RenderDraw.hpp"
#include "CommandBuffer/CommandQueue.hpp"
#include "Panda/Thread/Semaphore.hpp"

namespace Panda {

class Miren {
public:
    static void initialize();
    static void terminate();
    // MARK: - Command buffer
    static ShaderHandle createShader(const char *vertexPath, const char *fragmentPath);
    static void deleteShader(ShaderHandle handle);
    static TextureHandle createTextureFromFile(const char *path);
    static TextureHandle createTextureFromPixels(void *pixels, int width, int height);
    static void deleteTexture(TextureHandle handle);
    static IndexBufferHandle createIndexBuffer(void *indices, BufferElementType elementType, size_t count);
    static IndexBufferHandle createDynamicIndexBuffer(void *indices, BufferElementType elementType, size_t count);
    static void updateDynamicIndexBuffer(IndexBufferHandle, void *indices, size_t count);
    static void deleteIndexBuffer(IndexBufferHandle handle);
    static VertexBufferHandle createVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle);
    static VertexBufferHandle createDynamicVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle);
    static void updateDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size);
    static void deleteVertexBuffer(VertexBufferHandle handle);
    static VertexLayoutHandle createVertexLayout(VertexBufferLayoutData data);
    static void deleteVertexLayout(VertexLayoutHandle handle);
    // MARK: - Encoder setup
    static void setUniform(ShaderHandle handle, const char *name, void *value, UniformDataType type);
    static void setVertexBuffer(VertexBufferHandle handle);
    static void setIndexBuffer(IndexBufferHandle handle, void *offset, size_t count);
    static void setShader(ShaderHandle handle);
    static void setTexture(TextureHandle textureHandle, uint32_t slot);
    /// Submit draw call
    static void submit();
    /// Submit not indexed triangles draw call
    static void submitPrimitives(uint32_t elements);
    // MARK: - Main functions
    /// Process all requests to gpu (from rendering thread)
    static void renderFrame();
    /// Frame processing finished (from app thread). Wait for renderer to finish rendering frame
    static void frame();

    // TODO: - Remove
    static void renderSemaphoreWait();
    static void renderSemaphorePost();

private:
    static bool needToIntialize;

    static void rendererExecuteCommands();
    static Semaphore rendererSemaphore;
    static Frame s_frame;
    static RendererI *s_context;
    static CommandQueue s_commandQueue;
    static MirenHandleAllocator s_shadersHandleAlloc;
    static MirenHandleAllocator s_texturesHandleAlloc;
    static MirenHandleAllocator s_vertexLayoutsHandleAlloc;
    static MirenHandleAllocator s_vertexBuffersHandleAlloc;
    static MirenHandleAllocator s_indexBuffersHandleAlloc;
};

} // namespace Panda
