//
// Created by Admin on 05.03.2022.
//

#pragma once

#include "Panda/Renderer/Vertex.hpp"
#include "Panda/Renderer/VertexBufferLayoutData.hpp"
#include "Panda/Renderer/RendererI.hpp"

namespace Panda {

enum class RendererCommandType {
    CreateShader,
    DestroyShader,
    CreateTexture,
    DestroyTexture,
    CreateIndexBuffer,
    CreateDynamicIndexBuffer,
    UpdateDynamicIndexBuffer,
    DestroyIndexBuffer,
    CreateVertexBuffer,
    CreateDynamicVertexBuffer,
    UpdateDynamicVertexBuffer,
    DestroyVertexBuffer,
    CreateVertexLayout,
    DestroyVertexLayout
};

struct RendererCommand {
    RendererCommandType type;

    RendererCommand(RendererCommandType type)
        : type(type) {}
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

struct DeleteShaderCommand : RendererCommand {
    ShaderHandle handle;

    DeleteShaderCommand(ShaderHandle handle)
        : RendererCommand(RendererCommandType::DestroyShader)
        , handle(handle) {}
};

struct CreateTextureCommand : RendererCommand {
    const char *path;
    TextureHandle handle;

    CreateTextureCommand(TextureHandle handle, const char *path)
        : RendererCommand(RendererCommandType::CreateTexture)
        , handle(handle)
        , path(path) {}
};

struct DeleteTextureCommand : RendererCommand {
    TextureHandle handle;

    DeleteTextureCommand(TextureHandle handle)
        : RendererCommand(RendererCommandType::DestroyTexture)
        , handle(handle) {}
};

struct CreateIndexBufferCommand : RendererCommand {
    uint32_t *indices;
    uint32_t count;
    IndexBufferHandle handle;

    CreateIndexBufferCommand(IndexBufferHandle handle, uint32_t *indices, uint32_t count)
        : RendererCommand(RendererCommandType::CreateIndexBuffer)
        , handle(handle)
        , indices(indices)
        , count(count) {}
};

struct CreateDynamicIndexBufferCommand : RendererCommand {
    uint32_t *indices;
    uint32_t count;
    IndexBufferHandle handle;

    CreateDynamicIndexBufferCommand(IndexBufferHandle handle, uint32_t *indices, uint32_t count)
        : RendererCommand(RendererCommandType::CreateDynamicIndexBuffer)
        , handle(handle)
        , indices(indices)
        , count(count) {}
};

struct UpdateDynamicIndexBufferCommand : RendererCommand {
    IndexBufferHandle handle;
    uint32_t *indices;
    uint32_t count;

    UpdateDynamicIndexBufferCommand(IndexBufferHandle handle, uint32_t *indices, uint32_t count)
        : RendererCommand(RendererCommandType::UpdateDynamicIndexBuffer)
        , handle(handle)
        , indices(indices)
        , count(count) {}
};

struct DeleteIndexBufferCommand : RendererCommand {
    IndexBufferHandle handle;

    DeleteIndexBufferCommand(IndexBufferHandle handle)
        : RendererCommand(RendererCommandType::DestroyIndexBuffer)
        , handle(handle) {}
};

struct CreateVertexBufferCommand : RendererCommand {
    VertexBufferHandle handle;
    VertexLayoutHandle layoutHandle;
    void *data;
    uint32_t size;

    CreateVertexBufferCommand(VertexBufferHandle handle, void *data, uint32_t size, VertexLayoutHandle layoutHandle)
        : RendererCommand(RendererCommandType::CreateVertexBuffer)
        , handle(handle)
        , layoutHandle(layoutHandle)
        , data(data)
        , size(size) {}
};

struct CreateDynamicVertexBufferCommand : RendererCommand {
    VertexBufferHandle handle;
    VertexLayoutHandle layoutHandle;
    void *data;
    uint32_t size;

    CreateDynamicVertexBufferCommand(VertexBufferHandle handle, void *data, uint32_t size, VertexLayoutHandle layoutHandle)
        : RendererCommand(RendererCommandType::CreateDynamicVertexBuffer)
        , handle(handle)
        , layoutHandle(layoutHandle)
        , data(data)
        , size(size) {}
};

struct UpdateDynamicVertexBufferCommand : RendererCommand {
    VertexBufferHandle handle;
    void *data;
    uint32_t size;

    UpdateDynamicVertexBufferCommand(VertexBufferHandle handle, void *data, uint32_t size)
        : RendererCommand(RendererCommandType::UpdateDynamicVertexBuffer)
        , handle(handle)
        , data(data)
        , size(size) {}
};

struct DeleteVertexBufferCommand : RendererCommand {
    VertexBufferHandle handle;

    DeleteVertexBufferCommand(VertexBufferHandle handle)
        : RendererCommand(RendererCommandType::DestroyVertexBuffer)
        , handle(handle) {}
};

struct CreateVertexLayoutCommand : RendererCommand {
    VertexBufferLayoutData data;
    VertexLayoutHandle handle;

    CreateVertexLayoutCommand(VertexLayoutHandle handle, VertexBufferLayoutData data)
        : RendererCommand(RendererCommandType::CreateVertexLayout)
        , handle(handle)
        , data(std::move(data)) {}
};

struct DeleteVertexLayoutCommand : RendererCommand {
    VertexLayoutHandle handle;

    DeleteVertexLayoutCommand(VertexLayoutHandle handle)
        : RendererCommand(RendererCommandType::DestroyVertexLayout)
        , handle(handle) {}
};

} // namespace Panda