//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Panda/Renderer/IRenderer.hpp"
#include "Panda/Renderer/IShader.hpp"
#include "Panda/Renderer/ITexture.hpp"
#include "Panda/Renderer/IndexBuffer.hpp"
#include "Panda/Renderer/IVertexBuffer.hpp"
#include "Panda/Renderer/VertexBufferLayout.hpp"

namespace Panda {

enum class RendererApi {
    None = 0,
    OpenGL = 1,
    // Bgfx = 2,
    // Vulkan = 3
} const currentRendererApi(RendererApi::OpenGL);

class RendererInit {
public:
    static IRenderer *getRenderer();
    static IShader *createShader(const char *vertexPath, const char *fragmentPath);
    static ITexture *createTexture(const char *path);
    static IndexBuffer *createIndexBuffer(unsigned int *indices, unsigned int count, bool isDynamic);
    static IVertexBuffer *createVertexBuffer(Vertex *vertices, unsigned int count, bool isDynamic);
    static IVertexBuffer *createVertexBuffer(float *data, unsigned int count, bool isDynamic, VertexBufferLayout *layout);
    static VertexBufferLayout *createVertexBufferLayout();
};

} // namespace Panda
