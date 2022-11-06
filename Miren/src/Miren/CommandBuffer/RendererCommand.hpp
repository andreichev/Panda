//
// Created by Admin on 05.03.2022.
//

#pragma once

#include "Miren/Vertex.hpp"
#include "Miren/VertexBufferLayoutData.hpp"
#include "Miren/RendererI.hpp"

#include <Foundation/CommandBuffer.hpp>

namespace Miren {

enum RendererCommandType {
    RendererInit,
    CreateShader,
    DestroyShader,
    CreateTextureFromFile,
    CreateRGBATextureFromPixelsBuffer,
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

struct CreateShaderCommand : Foundation::CommandBuffer::Command {
    const char *vertexPath;
    const char *fragmentPath;
    ShaderHandle handle;

    CreateShaderCommand(ShaderHandle handle, const char *vertexPath, const char *fragmentPath)
        : Command(RendererCommandType::CreateShader)
        , handle(handle)
        , vertexPath(vertexPath)
        , fragmentPath(fragmentPath) {}
};

struct DeleteShaderCommand : Foundation::CommandBuffer::Command {
    ShaderHandle handle;

    DeleteShaderCommand(ShaderHandle handle)
        : Command(RendererCommandType::DestroyShader)
        , handle(handle) {}
};

struct CreateTextureFromFileCommand : Foundation::CommandBuffer::Command {
    const char *path;
    TextureHandle handle;

    CreateTextureFromFileCommand(TextureHandle handle, const char *path)
        : Command(RendererCommandType::CreateTextureFromFile)
        , handle(handle)
        , path(path) {}
};

struct CreateRGBATextureFromPixelsCommand : Foundation::CommandBuffer::Command {
    void *pixels;
    int width, height;
    TextureHandle handle;

    CreateRGBATextureFromPixelsCommand(TextureHandle handle, void *pixels, int width, int height)
        : Command(RendererCommandType::CreateRGBATextureFromPixelsBuffer)
        , handle(handle)
        , pixels(pixels)
        , width(width)
        , height(height) {}
};

struct DeleteTextureCommand : Foundation::CommandBuffer::Command {
    TextureHandle handle;

    DeleteTextureCommand(TextureHandle handle)
        : Command(RendererCommandType::DestroyTexture)
        , handle(handle) {}
};

struct CreateIndexBufferCommand : Foundation::CommandBuffer::Command {
    IndexBufferHandle handle;
    void *indices;
    BufferElementType elementType;
    size_t count;

    CreateIndexBufferCommand(IndexBufferHandle handle, void *indices, BufferElementType elementType, size_t count)
        : Command(RendererCommandType::CreateIndexBuffer)
        , handle(handle)
        , indices(indices)
        , elementType(elementType)
        , count(count) {}
};

struct CreateDynamicIndexBufferCommand : Foundation::CommandBuffer::Command {
    IndexBufferHandle handle;
    void *indices;
    BufferElementType elementType;
    size_t count;

    CreateDynamicIndexBufferCommand(IndexBufferHandle handle, void *indices, BufferElementType elementType, size_t count)
        : Command(RendererCommandType::CreateDynamicIndexBuffer)
        , handle(handle)
        , indices(indices)
        , elementType(elementType)
        , count(count) {}
};

struct UpdateDynamicIndexBufferCommand : Foundation::CommandBuffer::Command {
    IndexBufferHandle handle;
    void *indices;
    size_t count;

    UpdateDynamicIndexBufferCommand(IndexBufferHandle handle, void *indices, size_t count)
        : Command(RendererCommandType::UpdateDynamicIndexBuffer)
        , handle(handle)
        , indices(indices)
        , count(count) {}
};

struct DeleteIndexBufferCommand : Foundation::CommandBuffer::Command {
    IndexBufferHandle handle;

    DeleteIndexBufferCommand(IndexBufferHandle handle)
        : Command(RendererCommandType::DestroyIndexBuffer)
        , handle(handle) {}
};

struct CreateVertexBufferCommand : Foundation::CommandBuffer::Command {
    VertexBufferHandle handle;
    VertexLayoutHandle layoutHandle;
    void *data;
    uint32_t size;

    CreateVertexBufferCommand(VertexBufferHandle handle, void *data, uint32_t size, VertexLayoutHandle layoutHandle)
        : Command(RendererCommandType::CreateVertexBuffer)
        , handle(handle)
        , layoutHandle(layoutHandle)
        , data(data)
        , size(size) {}
};

struct CreateDynamicVertexBufferCommand : Foundation::CommandBuffer::Command {
    VertexBufferHandle handle;
    VertexLayoutHandle layoutHandle;
    void *data;
    uint32_t size;

    CreateDynamicVertexBufferCommand(VertexBufferHandle handle, void *data, uint32_t size, VertexLayoutHandle layoutHandle)
        : Command(RendererCommandType::CreateDynamicVertexBuffer)
        , handle(handle)
        , layoutHandle(layoutHandle)
        , data(data)
        , size(size) {}
};

struct UpdateDynamicVertexBufferCommand : Foundation::CommandBuffer::Command {
    VertexBufferHandle handle;
    void *data;
    uint32_t size;

    UpdateDynamicVertexBufferCommand(VertexBufferHandle handle, void *data, uint32_t size)
        : Command(RendererCommandType::UpdateDynamicVertexBuffer)
        , handle(handle)
        , data(data)
        , size(size) {}
};

struct DeleteVertexBufferCommand : Foundation::CommandBuffer::Command {
    VertexBufferHandle handle;

    DeleteVertexBufferCommand(VertexBufferHandle handle)
        : Command(RendererCommandType::DestroyVertexBuffer)
        , handle(handle) {}
};

struct CreateVertexLayoutCommand : Foundation::CommandBuffer::Command {
    VertexBufferLayoutData data;
    VertexLayoutHandle handle;

    CreateVertexLayoutCommand(VertexLayoutHandle handle, VertexBufferLayoutData data)
        : Command(RendererCommandType::CreateVertexLayout)
        , handle(handle)
        , data(data) {}
};

struct DeleteVertexLayoutCommand : Foundation::CommandBuffer::Command {
    VertexLayoutHandle handle;

    DeleteVertexLayoutCommand(VertexLayoutHandle handle)
        : Command(RendererCommandType::DestroyVertexLayout)
        , handle(handle) {}
};

} // namespace Miren