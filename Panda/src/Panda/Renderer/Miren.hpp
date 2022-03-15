//
// Created by Admin on 10.03.2022.
//

#pragma once

#include "RendererI.hpp"
#include "CommandBuffer/Frame.hpp"
#include "CommandBuffer/RenderDraw.hpp"
#include "CommandBuffer/CommandQueue.hpp"

namespace Panda {

class Miren {
public:
    static void initialize(GSize size);
    static ShaderHandle createShader(const char *vertexPath, const char *fragmentPath);
    static TextureHandle createTexture(const char *path);
    static IndexBufferHandle createIndexBuffer(unsigned int *indices, unsigned int count, bool isDynamic);
    static VertexBufferHandle createVertexBuffer(Vertex *vertices, unsigned int count, bool isDynamic);
    static VertexBufferHandle createVertexBuffer(float *data, unsigned int count, bool isDynamic, VertexLayoutHandle layoutHandle);
    static VertexLayoutHandle createVertexLayout(VertexBufferLayoutData data);
    static void setUniform(ShaderHandle handle, const char *name, void *value, uint16_t size);
    static void setVertexBuffer(VertexBufferHandle handle);
    static void setIndexBuffer(IndexBufferHandle handle, uint32_t count);
    static void setShader(ShaderHandle handle);
    static void setTexture(TextureHandle textureHandle, uint32_t slot);
    static void submit(ShaderHandle shader, VertexBufferHandle vertexBuffer, IndexBufferHandle indexBuffer, uint32_t indicesCount);
    static void submit();
    static void beginFrameProcessing();
    static void endFrameProcessing();
    static void renderFrame();

private:
    static void rendererExecuteCommands();

    static Frame s_frame;
    static RendererI *s_context;
    static CommandQueue s_commandQueue;
    static int s_shadersCount;
    static int s_texturesCount;
    static int s_vertexLayoutsCount;
    static int s_vertexBuffersCount;
    static int s_indexBuffersCount;
};

} // namespace Panda