//
// Created by Admin on 10.03.2022.
//

#pragma once

#include "MirenHandleAllocator.hpp"
#include "RendererI.hpp"
#include "CommandBuffer/Frame.hpp"
#include "CommandBuffer/RenderDraw.hpp"
#include "CommandBuffer/CommandQueue.hpp"

namespace Panda {

class Miren {
public:
    static void initialize(GSize size);
    static void terminate();
    // MARK: - Command buffer
    static ShaderHandle createShader(const char *vertexPath, const char *fragmentPath);
    static void deleteShader(ShaderHandle handle);
    static TextureHandle createTexture(const char *path);
    static void deleteTexture(TextureHandle handle);
    static IndexBufferHandle createIndexBuffer(uint32_t *indices, uint32_t count);
    static IndexBufferHandle createDynamicIndexBuffer(uint32_t *indices, uint32_t count);
    static void updateDynamicIndexBuffer(IndexBufferHandle, uint32_t *indices, uint32_t count);
    static void deleteIndexBuffer(IndexBufferHandle handle);
    static VertexBufferHandle createVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle);
    static VertexBufferHandle createDynamicVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle);
    static void updateDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size);
    static void deleteVertexBuffer(VertexBufferHandle handle);
    static VertexLayoutHandle createVertexLayout(VertexBufferLayoutData data);
    static void deleteVertexLayout(VertexLayoutHandle handle);
    // MARK: - Encoder setup
    static void setUniform(ShaderHandle handle, const char *name, void *value, uint16_t size);
    static void setVertexBuffer(VertexBufferHandle handle);
    static void setIndexBuffer(IndexBufferHandle handle, uint32_t count);
    static void setShader(ShaderHandle handle);
    static void setTexture(TextureHandle textureHandle, uint32_t slot);
    static void submit();
    static void submitPrimitives(uint32_t elements);
    static void beginFrameProcessing();
    static void endFrameProcessing();
    static void renderFrame();

private:
    static void rendererExecuteCommands();

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
