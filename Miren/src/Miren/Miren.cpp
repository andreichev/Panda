//
// Created by Admin on 10.03.2022.
//

#include "Miren/Miren_p.hpp"

#include <Foundation/Foundation.hpp>
#include <Foundation/Allocator.hpp>
#include <Foundation/Semaphore.hpp>
#include <Foundation/Logger.hpp>

namespace Miren {

static Context *s_context = nullptr;
static const int CONTEXT_ALIGNMENT = 64;

void initialize() {
    s_context = ALIGNED_NEW(Foundation::getAllocator(), Context, CONTEXT_ALIGNMENT);
    s_context->init();
}

void terminate() {
    s_context->shutdown();
    delete s_context->m_renderer;
    ALIGNED_DELETE(Foundation::getAllocator(), s_context, CONTEXT_ALIGNMENT);
}

ShaderHandle createShader(const char *vertexPath, const char *fragmentPath) {
    ShaderHandle handle = s_context->m_shadersHandleAlloc.alloc();
    s_context->m_commandQueue.post(new CreateShaderCommand(handle, vertexPath, fragmentPath));
    return handle;
}

void deleteShader(ShaderHandle handle) {
    s_context->m_shadersHandleAlloc.free(handle);
    s_context->m_commandQueue.post(new DeleteShaderCommand(handle));
}

TextureHandle createTextureFromFile(const char *path) {
    TextureHandle handle = s_context->m_texturesHandleAlloc.alloc();
    s_context->m_commandQueue.post(new CreateTextureFromFileCommand(handle, path));
    return handle;
}

TextureHandle createTextureFromPixels(void *pixels, int width, int height) {
    TextureHandle handle = s_context->m_texturesHandleAlloc.alloc();
    s_context->m_commandQueue.post(new CreateRGBATextureFromPixelsCommand(handle, pixels, width, height));
    return handle;
}

void deleteTexture(TextureHandle handle) {
    s_context->m_texturesHandleAlloc.free(handle);
    s_context->m_commandQueue.post(new DeleteTextureCommand(handle));
}

IndexBufferHandle createIndexBuffer(void *indices, BufferElementType elementType, size_t count) {
    IndexBufferHandle handle = s_context->m_indexBuffersHandleAlloc.alloc();
    s_context->m_commandQueue.post(new CreateIndexBufferCommand(handle, indices, elementType, count));
    return handle;
}

IndexBufferHandle createDynamicIndexBuffer(void *indices, BufferElementType elementType, size_t count) {
    IndexBufferHandle handle = s_context->m_indexBuffersHandleAlloc.alloc();
    s_context->m_commandQueue.post(new CreateDynamicIndexBufferCommand(handle, indices, elementType, count));
    return handle;
}

void updateDynamicIndexBuffer(IndexBufferHandle handle, void *indices, size_t count) {
    s_context->m_commandQueue.post(new UpdateDynamicIndexBufferCommand(handle, indices, count));
}

void deleteIndexBuffer(IndexBufferHandle handle) {
    s_context->m_indexBuffersHandleAlloc.free(handle);
    s_context->m_commandQueue.post(new DeleteIndexBufferCommand(handle));
}

VertexBufferHandle createVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
    VertexBufferHandle handle = s_context->m_vertexBuffersHandleAlloc.alloc();
    s_context->m_commandQueue.post(new CreateVertexBufferCommand(handle, data, size, layoutHandle));
    return handle;
}

VertexBufferHandle createDynamicVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
    VertexBufferHandle handle = s_context->m_vertexBuffersHandleAlloc.alloc();
    s_context->m_commandQueue.post(new CreateDynamicVertexBufferCommand(handle, data, size, layoutHandle));
    return handle;
}

void updateDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size) {
    s_context->m_commandQueue.post(new UpdateDynamicVertexBufferCommand(handle, data, size));
}

void deleteVertexBuffer(VertexBufferHandle handle) {
    s_context->m_vertexBuffersHandleAlloc.free(handle);
    s_context->m_commandQueue.post(new DeleteVertexBufferCommand(handle));
}

VertexLayoutHandle createVertexLayout(VertexBufferLayoutData data) {
    VertexLayoutHandle handle = s_context->m_vertexLayoutsHandleAlloc.alloc();
    s_context->m_commandQueue.post(new CreateVertexLayoutCommand(handle, data));
    return handle;
}

void deleteVertexLayout(VertexLayoutHandle handle) {
    s_context->m_vertexLayoutsHandleAlloc.free(handle);
    s_context->m_commandQueue.post(new DeleteVertexLayoutCommand(handle));
}

TransientVertexBuffer *createTransientVertexBuffer(uint32_t size, VertexBufferLayoutData layout) {
    TransientVertexBuffer *tvb = nullptr;
    VertexBufferHandle handle = s_context->m_vertexBuffersHandleAlloc.alloc();

    uint16_t stride = 0;
    VertexLayoutHandle layoutHandle = createVertexLayout(layout);
    stride = layout.m_Stride;

    CreateDynamicVertexBufferCommand *cmd = new CreateDynamicVertexBufferCommand(handle, nullptr, size, layoutHandle);
    s_context->m_commandQueue.post(cmd);

    tvb = (TransientVertexBuffer *)ALLOC(Foundation::getAllocator(), size);
    tvb->data = (uint8_t *)tvb + sizeof(TransientVertexBuffer);
    tvb->size = size;
    tvb->startVertex = 0;
    tvb->stride = stride;
    tvb->handle = handle;
    tvb->layoutHandle = layoutHandle;

    return tvb;
}

void allocTransientVertexBuffer(TransientVertexBuffer *buffer, int32_t count, const VertexLayoutHandle layoutHandle) {}

TransientIndexBuffer *createTransientIndexBuffer(uint32_t _size) {
    TransientIndexBuffer *tib = NULL;

    IndexBufferHandle handle = s_context->m_indexBuffersHandleAlloc.alloc();
    RendererCommand *cmd = new CreateDynamicIndexBufferCommand(handle, nullptr, BufferElementType::UnsignedInt, _size / 4);
    s_context->m_commandQueue.post(cmd);

    const uint32_t size = sizeof(TransientIndexBuffer) + _size;
    tib = (TransientIndexBuffer *)ALLOC(Foundation::getAllocator(), size);
    tib->data = (uint8_t *)tib + sizeof(TransientIndexBuffer);
    tib->size = _size;
    tib->handle = handle;

    return tib;
}

void renderFrame() {
    if (s_context->m_frame.getDrawCallsCount() == 0) {
        return;
    }
    rendererExecuteCommands();
    if (s_context->m_renderer == nullptr) {
        return;
    }
    s_context->m_renderer->clear();
    renderSemaphoreWait();

    for (int i = 0; i < s_context->m_frame.getDrawCallsCount(); i++) {
        RenderDraw &draw = s_context->m_frame.getDrawCalls()[i];
        if (draw.m_isSubmitted == false) {
            continue;
        }
        for (size_t u = 0; u < draw.m_uniformsCount; u++) {
            Uniform &uniform = draw.m_uniformBuffer[u];
            s_context->m_renderer->setUniform(uniform);
        }
        for (size_t t = 0; t < draw.m_textureBindingsCount; t++) {
            TextureBinding &textureBinding = draw.m_textureBindings[t];
            s_context->m_renderer->setTexture(textureBinding.m_handle, textureBinding.m_slot);
        }
        s_context->m_renderer->submit(&draw);
    }
    s_context->m_renderer->flip();
    s_context->m_frame.reset();
    renderSemaphorePost();
}

void rendererExecuteCommands() {
    const RendererCommand *command;
    if (s_context->m_renderer == nullptr) {
        command = s_context->m_commandQueue.poll();
        if (command != nullptr) {
            ASSERT(command->type == RendererCommandType::RendererInit, "First command in command buffer should be RendererInit");
            s_context->m_renderer = new RendererOpenGL();
            s_context->m_commandQueue.release(command);
        }
    }

    if (s_context->m_renderer == nullptr) {
        return;
    }

    while ((command = s_context->m_commandQueue.poll()) != nullptr) {
        switch (command->type) {
            case RendererCommandType::RendererInit: {
                break;
            }
            case RendererCommandType::CreateShader: {
                const CreateShaderCommand *cmd = static_cast<const CreateShaderCommand *>(command);
                s_context->m_renderer->createShader(cmd->handle, cmd->vertexPath, cmd->fragmentPath);
                break;
            }
            case RendererCommandType::DestroyShader: {
                const DeleteShaderCommand *cmd = static_cast<const DeleteShaderCommand *>(command);
                s_context->m_renderer->deleteShader(cmd->handle);
                break;
            }
            case RendererCommandType::CreateTextureFromFile: {
                const CreateTextureFromFileCommand *cmd = static_cast<const CreateTextureFromFileCommand *>(command);
                s_context->m_renderer->createTextureFromFile(cmd->handle, cmd->path);
                break;
            }
            case RendererCommandType::CreateRGBATextureFromPixelsBuffer: {
                const CreateRGBATextureFromPixelsCommand *cmd = static_cast<const CreateRGBATextureFromPixelsCommand *>(command);
                s_context->m_renderer->createRGBATextureFromPixels(cmd->handle, cmd->pixels, cmd->width, cmd->height);
                break;
            }
            case RendererCommandType::DestroyTexture: {
                const DeleteTextureCommand *cmd = static_cast<const DeleteTextureCommand *>(command);
                s_context->m_renderer->deleteTexture(cmd->handle);
                break;
            }
            case RendererCommandType::CreateIndexBuffer: {
                const CreateIndexBufferCommand *cmd = static_cast<const CreateIndexBufferCommand *>(command);
                s_context->m_renderer->createIndexBuffer(cmd->handle, cmd->indices, cmd->elementType, cmd->count);
                break;
            }
            case RendererCommandType::CreateDynamicIndexBuffer: {
                const CreateDynamicIndexBufferCommand *cmd = static_cast<const CreateDynamicIndexBufferCommand *>(command);
                s_context->m_renderer->createDynamicIndexBuffer(cmd->handle, cmd->indices, cmd->elementType, cmd->count);
                break;
            }
            case RendererCommandType::UpdateDynamicIndexBuffer: {
                const UpdateDynamicIndexBufferCommand *cmd = static_cast<const UpdateDynamicIndexBufferCommand *>(command);
                s_context->m_renderer->updateDynamicIndexBuffer(cmd->handle, cmd->indices, cmd->count);
                break;
            }
            case RendererCommandType::DestroyIndexBuffer: {
                const DeleteIndexBufferCommand *cmd = static_cast<const DeleteIndexBufferCommand *>(command);
                s_context->m_renderer->deleteIndexBuffer(cmd->handle);
                break;
            }
            case RendererCommandType::CreateVertexBuffer: {
                const CreateVertexBufferCommand *cmd = static_cast<const CreateVertexBufferCommand *>(command);
                s_context->m_renderer->createVertexBuffer(cmd->handle, cmd->data, cmd->size, cmd->layoutHandle);
                break;
            }
            case RendererCommandType::CreateDynamicVertexBuffer: {
                const CreateDynamicVertexBufferCommand *cmd = static_cast<const CreateDynamicVertexBufferCommand *>(command);
                s_context->m_renderer->createDynamicVertexBuffer(cmd->handle, cmd->data, cmd->size, cmd->layoutHandle);
                break;
            }
            case RendererCommandType::UpdateDynamicVertexBuffer: {
                const UpdateDynamicVertexBufferCommand *cmd = static_cast<const UpdateDynamicVertexBufferCommand *>(command);
                s_context->m_renderer->updateDynamicVertexBuffer(cmd->handle, cmd->data, cmd->size);
                break;
            }
            case RendererCommandType::DestroyVertexBuffer: {
                const DeleteVertexBufferCommand *cmd = static_cast<const DeleteVertexBufferCommand *>(command);
                s_context->m_renderer->deleteVertexBuffer(cmd->handle);
                break;
            }
            case RendererCommandType::CreateVertexLayout: {
                const CreateVertexLayoutCommand *cmd = static_cast<const CreateVertexLayoutCommand *>(command);
                s_context->m_renderer->createVertexLayout(cmd->handle, cmd->data);
                break;
            }
            case RendererCommandType::DestroyVertexLayout: {
                const DeleteVertexLayoutCommand *cmd = static_cast<const DeleteVertexLayoutCommand *>(command);
                s_context->m_renderer->deleteVertexLayout(cmd->handle);
                break;
            }
        }
        s_context->m_commandQueue.release(command);
    }
}

void renderSemaphoreWait() {
    s_context->m_rendererSemaphore.wait();
}

void renderSemaphorePost() {
    s_context->m_rendererSemaphore.post();
}

// TODO: - Implement, add swapping frames
void frame() {
    renderSemaphoreWait();
}

void setState(uint32_t state) {
    s_context->m_frame.setState(state);
}

void setScissorRect(Rect rect) {
    s_context->m_frame.setScissorRect(rect);
}

void setVertexBuffer(VertexBufferHandle handle) {
    s_context->m_frame.setVertexBuffer(handle);
}

void setIndexBuffer(IndexBufferHandle handle, void *offset, size_t count) {
    s_context->m_frame.setIndexBuffer(handle, offset, count);
}

void setShader(ShaderHandle handle) {
    s_context->m_frame.setShader(handle);
}

void setUniform(ShaderHandle handle, const char *name, void *value, UniformDataType type) {
    s_context->m_frame.setUniform(handle, name, value, type);
}

void setTexture(TextureHandle textureHandle, uint32_t slot) {
    s_context->m_frame.setTexture(textureHandle, slot);
}

void submit() {
    s_context->m_frame.submitCurrentDrawCall();
    s_context->m_frame.beginDrawCall();
}

void submitPrimitives(uint32_t elements) {
    s_context->m_frame.setIsIndexed(false);
    s_context->m_frame.setNumberOfElements(elements);
    s_context->m_frame.submitCurrentDrawCall();
    s_context->m_frame.beginDrawCall();
}

} // namespace Miren
