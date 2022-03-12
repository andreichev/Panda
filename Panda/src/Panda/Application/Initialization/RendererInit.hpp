//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Panda/Renderer/GraphicsContext.hpp"
#include "Panda/Renderer/Renderer.hpp"
#include "Panda/Renderer/Shader.hpp"
#include "Panda/Renderer/Texture.hpp"
#include "Panda/Renderer/IndexBuffer.hpp"
#include "Panda/Renderer/VertexBuffer.hpp"
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
    static GraphicsContext *getContext();
    static Renderer *getRenderer();
    static Shader *createShader(const char *vertexPath, const char *fragmentPath);
    static Texture *createTexture(const char *path);
    static IndexBuffer *createIndexBuffer(unsigned int *indices, unsigned int count, bool isDynamic);
    static VertexBuffer *createVertexBuffer(Vertex *vertices, unsigned int count, bool isDynamic);
    static VertexBuffer *createVertexBuffer(float *data, unsigned int count, bool isDynamic, VertexBufferLayout *layout);
    static VertexBufferLayout *createVertexBufferLayout();
};

} // namespace Panda
