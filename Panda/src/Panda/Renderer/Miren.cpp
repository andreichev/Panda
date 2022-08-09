//
// Created by Admin on 10.03.2022.
//

#include "pndpch.hpp"
#include "Miren.hpp"

#include "Platform/RendererImpl/OpenGL/RendererOpenGL.hpp"

namespace Panda {

bool Miren::needToIntialize = false;
RendererI *Miren::s_context = nullptr;
Frame Miren::s_frame = Frame();
constexpr uint32_t maxHandles = 1000;

MirenHandleAllocator Miren::s_shadersHandleAlloc(maxHandles);
MirenHandleAllocator Miren::s_texturesHandleAlloc(maxHandles);
MirenHandleAllocator Miren::s_vertexLayoutsHandleAlloc(maxHandles);
MirenHandleAllocator Miren::s_vertexBuffersHandleAlloc(maxHandles);
MirenHandleAllocator Miren::s_indexBuffersHandleAlloc(maxHandles);

CommandQueue Miren::s_commandQueue;
Semaphore Miren::rendererSemaphore;

void Miren::initialize() {
    Miren::needToIntialize = true;
}

void Miren::terminate() {
    delete s_context;
}

ShaderHandle Miren::createShader(const char *vertexPath, const char *fragmentPath) {
    ShaderHandle handle = s_shadersHandleAlloc.alloc();
    s_commandQueue.post(new CreateShaderCommand(handle, vertexPath, fragmentPath));
    return handle;
}

void Miren::deleteShader(ShaderHandle handle) {
    s_shadersHandleAlloc.free(handle);
    s_commandQueue.post(new DeleteShaderCommand(handle));
}

TextureHandle Miren::createTextureFromFile(const char *path) {
    TextureHandle handle = s_texturesHandleAlloc.alloc();
    s_commandQueue.post(new CreateTextureFromFileCommand(handle, path));
    return handle;
}

TextureHandle Miren::createTextureFromPixels(void *pixels, int width, int height) {
    TextureHandle handle = s_texturesHandleAlloc.alloc();
    s_commandQueue.post(new CreateRGBATextureFromPixelsCommand(handle, pixels, width, height));
    return handle;
}

void Miren::deleteTexture(TextureHandle handle) {
    s_texturesHandleAlloc.free(handle);
    s_commandQueue.post(new DeleteTextureCommand(handle));
}

IndexBufferHandle Miren::createIndexBuffer(void *indices, BufferElementType elementType, size_t count) {
    IndexBufferHandle handle = s_indexBuffersHandleAlloc.alloc();
    s_commandQueue.post(new CreateIndexBufferCommand(handle, indices, elementType, count));
    return handle;
}

IndexBufferHandle Miren::createDynamicIndexBuffer(void *indices, BufferElementType elementType, size_t count) {
    IndexBufferHandle handle = s_indexBuffersHandleAlloc.alloc();
    s_commandQueue.post(new CreateDynamicIndexBufferCommand(handle, indices, elementType, count));
    return handle;
}

void Miren::updateDynamicIndexBuffer(IndexBufferHandle handle, void *indices, size_t count) {
    s_commandQueue.post(new UpdateDynamicIndexBufferCommand(handle, indices, count));
}

void Miren::deleteIndexBuffer(IndexBufferHandle handle) {
    s_indexBuffersHandleAlloc.free(handle);
    s_commandQueue.post(new DeleteIndexBufferCommand(handle));
}

VertexBufferHandle Miren::createVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
    VertexBufferHandle handle = s_vertexBuffersHandleAlloc.alloc();
    s_commandQueue.post(new CreateVertexBufferCommand(handle, data, size, layoutHandle));
    return handle;
}

VertexBufferHandle Miren::createDynamicVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
    VertexBufferHandle handle = s_vertexBuffersHandleAlloc.alloc();
    s_commandQueue.post(new CreateDynamicVertexBufferCommand(handle, data, size, layoutHandle));
    return handle;
}

void Miren::updateDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size) {
    s_commandQueue.post(new UpdateDynamicVertexBufferCommand(handle, data, size));
}

void Miren::deleteVertexBuffer(VertexBufferHandle handle) {
    s_vertexBuffersHandleAlloc.free(handle);
    s_commandQueue.post(new DeleteVertexBufferCommand(handle));
}

VertexLayoutHandle Miren::createVertexLayout(VertexBufferLayoutData data) {
    VertexLayoutHandle handle = s_vertexLayoutsHandleAlloc.alloc();
    s_commandQueue.post(new CreateVertexLayoutCommand(handle, data));
    return handle;
}

void Miren::deleteVertexLayout(VertexLayoutHandle handle) {
    s_vertexLayoutsHandleAlloc.free(handle);
    s_commandQueue.post(new DeleteVertexLayoutCommand(handle));
}

void Miren::renderFrame() {
    if (s_context == nullptr) {
        if (needToIntialize) {
            s_context = new RendererOpenGL();
            needToIntialize = false;
        }
        return;
    }
    if (s_frame.getDrawCallsCount() == 0) {
        return;
    }
    s_context->clear();
    renderSemaphoreWait();
    rendererExecuteCommands();
    RenderDraw *draw;
    while ((draw = s_frame.popDrawCall()) != nullptr) {
        if (draw->m_isSubmitted == false) {
            s_frame.free(draw);
            continue;
        }
        while (draw->m_uniformBuffer.empty() == false) {
            Uniform &uniform = draw->m_uniformBuffer.front();
            s_context->setUniform(uniform);
            draw->m_uniformBuffer.pop();
        }
        while (draw->m_textureBindings.empty() == false) {
            TextureBinding &textureBinding = draw->m_textureBindings.front();
            s_context->setTexture(textureBinding.m_handle, textureBinding.m_slot);
            draw->m_textureBindings.pop();
        }
        if (draw->m_isIndexed) {
            s_context->submitIndexed(draw->m_shader, draw->m_vertexBuffer, draw->m_indexBuffer, draw->m_indicesOffset, draw->m_numIndices);
        } else {
            s_context->submitPrimitives(draw->m_shader, draw->m_vertexBuffer, draw->m_numElemets);
        }
        s_frame.free(draw);
    }
    s_context->flip();
    s_frame.beginDrawCall();
    renderSemaphorePost();
}

void Miren::rendererExecuteCommands() {
    const RendererCommand *command;
    while ((command = s_commandQueue.poll()) != nullptr) {
        switch (command->type) {
            case RendererCommandType::CreateShader: {
                const CreateShaderCommand *cmd = static_cast<const CreateShaderCommand *>(command);
                s_context->createShader(cmd->handle, cmd->vertexPath, cmd->fragmentPath);
                break;
            }
            case RendererCommandType::DestroyShader: {
                const DeleteShaderCommand *cmd = static_cast<const DeleteShaderCommand *>(command);
                s_context->deleteShader(cmd->handle);
                break;
            }
            case RendererCommandType::CreateTextureFromFile: {
                const CreateTextureFromFileCommand *cmd = static_cast<const CreateTextureFromFileCommand *>(command);
                s_context->createTextureFromFile(cmd->handle, cmd->path);
                break;
            }
            case RendererCommandType::CreateRGBATextureFromPixelsBuffer: {
                const CreateRGBATextureFromPixelsCommand *cmd = static_cast<const CreateRGBATextureFromPixelsCommand *>(command);
                s_context->createRGBATextureFromPixels(cmd->handle, cmd->pixels, cmd->width, cmd->height);
                break;
            }
            case RendererCommandType::DestroyTexture: {
                const DeleteTextureCommand *cmd = static_cast<const DeleteTextureCommand *>(command);
                s_context->deleteTexture(cmd->handle);
                break;
            }
            case RendererCommandType::CreateIndexBuffer: {
                const CreateIndexBufferCommand *cmd = static_cast<const CreateIndexBufferCommand *>(command);
                s_context->createIndexBuffer(cmd->handle, cmd->indices, cmd->elementType, cmd->count);
                break;
            }
            case RendererCommandType::CreateDynamicIndexBuffer: {
                const CreateDynamicIndexBufferCommand *cmd = static_cast<const CreateDynamicIndexBufferCommand *>(command);
                s_context->createDynamicIndexBuffer(cmd->handle, cmd->indices, cmd->elementType, cmd->count);
                break;
            }
            case RendererCommandType::UpdateDynamicIndexBuffer: {
                const UpdateDynamicIndexBufferCommand *cmd = static_cast<const UpdateDynamicIndexBufferCommand *>(command);
                s_context->updateDynamicIndexBuffer(cmd->handle, cmd->indices, cmd->count);
                break;
            }
            case RendererCommandType::DestroyIndexBuffer: {
                const DeleteIndexBufferCommand *cmd = static_cast<const DeleteIndexBufferCommand *>(command);
                s_context->deleteIndexBuffer(cmd->handle);
                break;
            }
            case RendererCommandType::CreateVertexBuffer: {
                const CreateVertexBufferCommand *cmd = static_cast<const CreateVertexBufferCommand *>(command);
                s_context->createVertexBuffer(cmd->handle, cmd->data, cmd->size, cmd->layoutHandle);
                break;
            }
            case RendererCommandType::CreateDynamicVertexBuffer: {
                const CreateDynamicVertexBufferCommand *cmd = static_cast<const CreateDynamicVertexBufferCommand *>(command);
                s_context->createDynamicVertexBuffer(cmd->handle, cmd->data, cmd->size, cmd->layoutHandle);
                break;
            }
            case RendererCommandType::UpdateDynamicVertexBuffer: {
                const UpdateDynamicVertexBufferCommand *cmd = static_cast<const UpdateDynamicVertexBufferCommand *>(command);
                s_context->updateDynamicVertexBuffer(cmd->handle, cmd->data, cmd->size);
                break;
            }
            case RendererCommandType::DestroyVertexBuffer: {
                const DeleteVertexBufferCommand *cmd = static_cast<const DeleteVertexBufferCommand *>(command);
                s_context->deleteVertexBuffer(cmd->handle);
                break;
            }
            case RendererCommandType::CreateVertexLayout: {
                const CreateVertexLayoutCommand *cmd = static_cast<const CreateVertexLayoutCommand *>(command);
                s_context->createVertexLayout(cmd->handle, cmd->data);
                break;
            }
            case RendererCommandType::DestroyVertexLayout: {
                const DeleteVertexLayoutCommand *cmd = static_cast<const DeleteVertexLayoutCommand *>(command);
                s_context->deleteVertexLayout(cmd->handle);
                break;
            }
        }
        s_commandQueue.release(command);
    }
}

void Miren::renderSemaphoreWait() {
    rendererSemaphore.wait();
}

void Miren::renderSemaphorePost() {
    rendererSemaphore.post();
}

// TODO: - Implement, add swapping frames
void Miren::frame() {
    renderSemaphoreWait();
}

void Miren::setVertexBuffer(VertexBufferHandle handle) {
    s_frame.setVertexBuffer(handle);
}

void Miren::setIndexBuffer(IndexBufferHandle handle, void *offset, size_t count) {
    s_frame.setIndexBuffer(handle, offset, count);
}

void Miren::setShader(ShaderHandle handle) {
    s_frame.setShader(handle);
}

void Miren::setUniform(ShaderHandle handle, const char *name, void *value, UniformDataType type) {
    s_frame.setUniform(handle, name, value, type);
}

void Miren::setTexture(TextureHandle textureHandle, uint32_t slot) {
    s_frame.setTexture(textureHandle, slot);
}

void Miren::submit() {
    s_frame.submitCurrentDrawCall();
    s_frame.beginDrawCall();
}

void Miren::submitPrimitives(uint32_t elements) {
    s_frame.setIsIndexed(false);
    s_frame.setNumberOfElements(elements);
    s_frame.submitCurrentDrawCall();
    s_frame.beginDrawCall();
}

} // namespace Panda
