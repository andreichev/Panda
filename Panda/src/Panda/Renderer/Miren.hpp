//
// Created by Admin on 10.03.2022.
//

#pragma once

#include "RendererI.hpp"
#include "CommandBuffer/CommandQueue.hpp"

namespace Panda {

class Miren {
public:
    Miren();
    static ShaderHandle createShader(const char *vertexPath, const char *fragmentPath);
    static TextureHandle createTexture(const char *path);
    static IndexBufferHandle createIndexBuffer(unsigned int *indices, unsigned int count, bool isDynamic);
    static VertexBufferHandle createVertexBuffer(Vertex *vertices, unsigned int count, bool isDynamic);
    static VertexBufferHandle createVertexBuffer(float *data, unsigned int count, bool isDynamic, VertexLayoutHandle layoutHandle);
    static VertexLayoutHandle createVertexLayout(VertexBufferLayoutData data);
    static void beginFrameProcessing();
    static void endFrameProcessing();
    static void renderFrame();

private:
    static void rendererExecuteCommands();

    static RendererI *s_context;
    static CommandQueue s_commandQueue;
    static int s_shadersCount;
    static int s_texturesCount;
    static int s_vertexLayoutsCount;
    static int s_vertexBuffersCount;
    static int s_indexBuffersCount;
};

} // namespace Panda