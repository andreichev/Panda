//
// Created by Admin on 05.03.2022.
//

#pragma once

#include "Panda/Renderer/Vertex.hpp"
#include "Panda/Renderer/VertexBufferLayoutData.hpp"
#include "Panda/Renderer/RendererI.hpp"

namespace Panda {

enum class RendererCommandType {
    CreateVertexLayout,
    CreateIndexBuffer,
    CreateVertexBuffer,
    // UpdateIndexBuffer,
    // UpdateVertexBuffer,
    CreateShader,
    CreateTexture,
    // CreateUniform,
    // DestroyVertexLayout,
    // DestroyIndexBuffer,
    // DestroyVertexBuffer,
    // DestroyShader,
    // DestroyTexture,
};

struct RendererCommand {
    RendererCommandType type;

    RendererCommand(RendererCommandType type)
        : type(type) {}
};

struct CreateVertexLayoutCommand : RendererCommand {
    VertexBufferLayoutData data;
    VertexLayoutHandle handle;

    CreateVertexLayoutCommand(VertexLayoutHandle handle, VertexBufferLayoutData data)
        : RendererCommand(RendererCommandType::CreateVertexLayout)
        , handle(handle)
        , data(std::move(data)) {}
};

struct CreateShaderCommand : RendererCommand {
    const char *vertexPath;
    const char *fragmentPath;
    ShaderHandle handle;

    CreateShaderCommand(ShaderHandle handle, const char *vertexPath, const char *fragmentPath)
        : RendererCommand(RendererCommandType::CreateShader)
        , handle(handle)
        , vertexPath(vertexPath)
        , fragmentPath(fragmentPath) {}
};

struct CreateTextureCommand : RendererCommand {
    const char *path;
    TextureHandle handle;

    CreateTextureCommand(TextureHandle handle, const char *path)
        : RendererCommand(RendererCommandType::CreateTexture)
        , handle(handle)
        , path(path) {}
};

struct CreateIndexBufferCommand : RendererCommand {
    unsigned int *indices;
    unsigned int count;
    bool isDynamic;
    IndexBufferHandle handle;

    CreateIndexBufferCommand(IndexBufferHandle handle, unsigned int *indices, unsigned int count, bool isDynamic)
        : RendererCommand(RendererCommandType::CreateIndexBuffer)
        , handle(handle)
        , indices(indices)
        , count(count)
        , isDynamic(isDynamic) {}
};

struct CreateVertexBufferCommand : RendererCommand {
    float *data;
    unsigned int count;
    bool isDynamic;
    VertexBufferHandle handle;
    VertexLayoutHandle layoutHandle;

    CreateVertexBufferCommand(VertexBufferHandle handle, float *data, unsigned int count, bool isDynamic, VertexLayoutHandle layoutHandle)
        : RendererCommand(RendererCommandType::CreateVertexBuffer)
        , handle(handle)
        , data(data)
        , count(count)
        , isDynamic(isDynamic)
        , layoutHandle(layoutHandle) {}
};

} // namespace Panda