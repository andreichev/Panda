//
// Created by Admin on 10.03.2022.
//

#include "Miren/Miren.hpp"

#include "RendererI.hpp"
#include "CommandBuffer/Frame.hpp"
#include "CommandBuffer/RenderDraw.hpp"
#include "CommandBuffer/CommandQueue.hpp"
#include "MirenStates.hpp"
#include "MirenHandleAllocator.hpp"

#include "Platform/RendererImpl/OpenGL/RendererOpenGL.hpp"

#include <Foundation/Foundation.hpp>
#include <Foundation/Allocator.hpp>
#include <Foundation/Semaphore.hpp>
#include <Foundation/Logger.hpp>

namespace Panda {

namespace Miren {

    struct Context {};

    // TODO: - Create context
    static RendererI *s_renderer = nullptr;
    static Frame s_frame = Frame();
    constexpr uint32_t maxHandles = 1000;

    static MirenHandleAllocator s_shadersHandleAlloc(maxHandles);
    static MirenHandleAllocator s_texturesHandleAlloc(maxHandles);
    static MirenHandleAllocator s_vertexLayoutsHandleAlloc(maxHandles);
    static MirenHandleAllocator s_vertexBuffersHandleAlloc(maxHandles);
    static MirenHandleAllocator s_indexBuffersHandleAlloc(maxHandles);

    static CommandQueue s_commandQueue;
    Semaphore rendererSemaphore;

    TransientVertexBuffer *createTransientVertexBuffer(uint32_t _size, VertexBufferLayoutData layout);
    TransientIndexBuffer *createTransientIndexBuffer(uint32_t _size);

    void initialize() {
        s_commandQueue.post(new RendererCommand(RendererCommandType::RendererInit));
        s_frame.m_transientVb = createTransientVertexBuffer(TRANSIENT_VERTEX_BUFFER_SIZE, VertexBufferLayoutData());
        s_frame.m_transientIb = createTransientIndexBuffer(TRANSIENT_INDEX_BUFFER_SIZE);
    }

    void terminate() {
        delete s_renderer;
    }

    ShaderHandle createShader(const char *vertexPath, const char *fragmentPath) {
        ShaderHandle handle = s_shadersHandleAlloc.alloc();
        s_commandQueue.post(new CreateShaderCommand(handle, vertexPath, fragmentPath));
        return handle;
    }

    void deleteShader(ShaderHandle handle) {
        s_shadersHandleAlloc.free(handle);
        s_commandQueue.post(new DeleteShaderCommand(handle));
    }

    TextureHandle createTextureFromFile(const char *path) {
        TextureHandle handle = s_texturesHandleAlloc.alloc();
        s_commandQueue.post(new CreateTextureFromFileCommand(handle, path));
        return handle;
    }

    TextureHandle createTextureFromPixels(void *pixels, int width, int height) {
        TextureHandle handle = s_texturesHandleAlloc.alloc();
        s_commandQueue.post(new CreateRGBATextureFromPixelsCommand(handle, pixels, width, height));
        return handle;
    }

    void deleteTexture(TextureHandle handle) {
        s_texturesHandleAlloc.free(handle);
        s_commandQueue.post(new DeleteTextureCommand(handle));
    }

    IndexBufferHandle createIndexBuffer(void *indices, BufferElementType elementType, size_t count) {
        IndexBufferHandle handle = s_indexBuffersHandleAlloc.alloc();
        s_commandQueue.post(new CreateIndexBufferCommand(handle, indices, elementType, count));
        return handle;
    }

    IndexBufferHandle createDynamicIndexBuffer(void *indices, BufferElementType elementType, size_t count) {
        IndexBufferHandle handle = s_indexBuffersHandleAlloc.alloc();
        s_commandQueue.post(new CreateDynamicIndexBufferCommand(handle, indices, elementType, count));
        return handle;
    }

    void updateDynamicIndexBuffer(IndexBufferHandle handle, void *indices, size_t count) {
        s_commandQueue.post(new UpdateDynamicIndexBufferCommand(handle, indices, count));
    }

    void deleteIndexBuffer(IndexBufferHandle handle) {
        s_indexBuffersHandleAlloc.free(handle);
        s_commandQueue.post(new DeleteIndexBufferCommand(handle));
    }

    VertexBufferHandle createVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
        VertexBufferHandle handle = s_vertexBuffersHandleAlloc.alloc();
        s_commandQueue.post(new CreateVertexBufferCommand(handle, data, size, layoutHandle));
        return handle;
    }

    VertexBufferHandle createDynamicVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
        VertexBufferHandle handle = s_vertexBuffersHandleAlloc.alloc();
        s_commandQueue.post(new CreateDynamicVertexBufferCommand(handle, data, size, layoutHandle));
        return handle;
    }

    void updateDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size) {
        s_commandQueue.post(new UpdateDynamicVertexBufferCommand(handle, data, size));
    }

    void deleteVertexBuffer(VertexBufferHandle handle) {
        s_vertexBuffersHandleAlloc.free(handle);
        s_commandQueue.post(new DeleteVertexBufferCommand(handle));
    }

    VertexLayoutHandle createVertexLayout(VertexBufferLayoutData data) {
        VertexLayoutHandle handle = s_vertexLayoutsHandleAlloc.alloc();
        s_commandQueue.post(new CreateVertexLayoutCommand(handle, data));
        return handle;
    }

    void deleteVertexLayout(VertexLayoutHandle handle) {
        s_vertexLayoutsHandleAlloc.free(handle);
        s_commandQueue.post(new DeleteVertexLayoutCommand(handle));
    }

    TransientVertexBuffer *createTransientVertexBuffer(uint32_t size, VertexBufferLayoutData layout) {
        TransientVertexBuffer *tvb = nullptr;
        VertexBufferHandle handle = s_vertexBuffersHandleAlloc.alloc();

        uint16_t stride = 0;
        VertexLayoutHandle layoutHandle = createVertexLayout(layout);
        stride = layout.m_Stride;

        CreateDynamicVertexBufferCommand *cmd = new CreateDynamicVertexBufferCommand(handle, nullptr, size, layoutHandle);
        s_commandQueue.post(cmd);

        tvb = (TransientVertexBuffer *)PND_ALLOC(getAllocator(), size);
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

        IndexBufferHandle handle = s_indexBuffersHandleAlloc.alloc();
        RendererCommand *cmd = new CreateDynamicIndexBufferCommand(handle, nullptr, BufferElementType::UnsignedInt, _size / 4);
        s_commandQueue.post(cmd);

        const uint32_t size = sizeof(TransientIndexBuffer) + _size;
        tib = (TransientIndexBuffer *)PND_ALLOC(getAllocator(), size);
        tib->data = (uint8_t *)tib + sizeof(TransientIndexBuffer);
        tib->size = _size;
        tib->handle = handle;

        return tib;
    }

    void rendererExecuteCommands();

    void renderFrame() {
        if (s_frame.getDrawCallsCount() == 0) {
            return;
        }
        rendererExecuteCommands();
        if (s_renderer == nullptr) {
            return;
        }
        s_renderer->clear();
        renderSemaphoreWait();
        RenderDraw *draw;
        while ((draw = s_frame.popDrawCall()) != nullptr) {
            if (draw->m_isSubmitted == false) {
                s_frame.free(draw);
                continue;
            }
            while (draw->m_uniformBuffer.empty() == false) {
                Uniform &uniform = draw->m_uniformBuffer.front();
                s_renderer->setUniform(uniform);
                draw->m_uniformBuffer.pop();
            }
            while (draw->m_textureBindings.empty() == false) {
                TextureBinding &textureBinding = draw->m_textureBindings.front();
                s_renderer->setTexture(textureBinding.m_handle, textureBinding.m_slot);
                draw->m_textureBindings.pop();
            }
            s_renderer->submit(draw);
            s_frame.free(draw);
        }
        s_renderer->flip();
        s_frame.beginDrawCall();
        renderSemaphorePost();
    }

    void rendererExecuteCommands() {
        const RendererCommand *command;
        if (s_renderer == nullptr) {
            command = s_commandQueue.poll();
            if (command != nullptr) {
                PND_ASSERT(command->type == RendererCommandType::RendererInit, "First command in command buffer should be RendererInit");
                s_renderer = new RendererOpenGL();
                s_commandQueue.release(command);
            }
        }

        if (s_renderer == nullptr) {
            return;
        }

        while ((command = s_commandQueue.poll()) != nullptr) {
            switch (command->type) {
                case RendererCommandType::CreateShader: {
                    const CreateShaderCommand *cmd = static_cast<const CreateShaderCommand *>(command);
                    s_renderer->createShader(cmd->handle, cmd->vertexPath, cmd->fragmentPath);
                    break;
                }
                case RendererCommandType::DestroyShader: {
                    const DeleteShaderCommand *cmd = static_cast<const DeleteShaderCommand *>(command);
                    s_renderer->deleteShader(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateTextureFromFile: {
                    const CreateTextureFromFileCommand *cmd = static_cast<const CreateTextureFromFileCommand *>(command);
                    s_renderer->createTextureFromFile(cmd->handle, cmd->path);
                    break;
                }
                case RendererCommandType::CreateRGBATextureFromPixelsBuffer: {
                    const CreateRGBATextureFromPixelsCommand *cmd = static_cast<const CreateRGBATextureFromPixelsCommand *>(command);
                    s_renderer->createRGBATextureFromPixels(cmd->handle, cmd->pixels, cmd->width, cmd->height);
                    break;
                }
                case RendererCommandType::DestroyTexture: {
                    const DeleteTextureCommand *cmd = static_cast<const DeleteTextureCommand *>(command);
                    s_renderer->deleteTexture(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateIndexBuffer: {
                    const CreateIndexBufferCommand *cmd = static_cast<const CreateIndexBufferCommand *>(command);
                    s_renderer->createIndexBuffer(cmd->handle, cmd->indices, cmd->elementType, cmd->count);
                    break;
                }
                case RendererCommandType::CreateDynamicIndexBuffer: {
                    const CreateDynamicIndexBufferCommand *cmd = static_cast<const CreateDynamicIndexBufferCommand *>(command);
                    s_renderer->createDynamicIndexBuffer(cmd->handle, cmd->indices, cmd->elementType, cmd->count);
                    break;
                }
                case RendererCommandType::UpdateDynamicIndexBuffer: {
                    const UpdateDynamicIndexBufferCommand *cmd = static_cast<const UpdateDynamicIndexBufferCommand *>(command);
                    s_renderer->updateDynamicIndexBuffer(cmd->handle, cmd->indices, cmd->count);
                    break;
                }
                case RendererCommandType::DestroyIndexBuffer: {
                    const DeleteIndexBufferCommand *cmd = static_cast<const DeleteIndexBufferCommand *>(command);
                    s_renderer->deleteIndexBuffer(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateVertexBuffer: {
                    const CreateVertexBufferCommand *cmd = static_cast<const CreateVertexBufferCommand *>(command);
                    s_renderer->createVertexBuffer(cmd->handle, cmd->data, cmd->size, cmd->layoutHandle);
                    break;
                }
                case RendererCommandType::CreateDynamicVertexBuffer: {
                    const CreateDynamicVertexBufferCommand *cmd = static_cast<const CreateDynamicVertexBufferCommand *>(command);
                    s_renderer->createDynamicVertexBuffer(cmd->handle, cmd->data, cmd->size, cmd->layoutHandle);
                    break;
                }
                case RendererCommandType::UpdateDynamicVertexBuffer: {
                    const UpdateDynamicVertexBufferCommand *cmd = static_cast<const UpdateDynamicVertexBufferCommand *>(command);
                    s_renderer->updateDynamicVertexBuffer(cmd->handle, cmd->data, cmd->size);
                    break;
                }
                case RendererCommandType::DestroyVertexBuffer: {
                    const DeleteVertexBufferCommand *cmd = static_cast<const DeleteVertexBufferCommand *>(command);
                    s_renderer->deleteVertexBuffer(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateVertexLayout: {
                    const CreateVertexLayoutCommand *cmd = static_cast<const CreateVertexLayoutCommand *>(command);
                    s_renderer->createVertexLayout(cmd->handle, cmd->data);
                    break;
                }
                case RendererCommandType::DestroyVertexLayout: {
                    const DeleteVertexLayoutCommand *cmd = static_cast<const DeleteVertexLayoutCommand *>(command);
                    s_renderer->deleteVertexLayout(cmd->handle);
                    break;
                }
            }
            s_commandQueue.release(command);
        }
    }

    void renderSemaphoreWait() {
        rendererSemaphore.wait();
    }

    void renderSemaphorePost() {
        rendererSemaphore.post();
    }

    // TODO: - Implement, add swapping frames
    void frame() {
        renderSemaphoreWait();
    }

    void setState(uint32_t state) {
        s_frame.setState(state);
    }

    void setScissorRect(Rect rect) {
        s_frame.setScissorRect(rect);
    }

    void setVertexBuffer(VertexBufferHandle handle) {
        s_frame.setVertexBuffer(handle);
    }

    void setIndexBuffer(IndexBufferHandle handle, void *offset, size_t count) {
        s_frame.setIndexBuffer(handle, offset, count);
    }

    void setShader(ShaderHandle handle) {
        s_frame.setShader(handle);
    }

    void setUniform(ShaderHandle handle, const char *name, void *value, UniformDataType type) {
        s_frame.setUniform(handle, name, value, type);
    }

    void setTexture(TextureHandle textureHandle, uint32_t slot) {
        s_frame.setTexture(textureHandle, slot);
    }

    void submit() {
        s_frame.submitCurrentDrawCall();
        s_frame.beginDrawCall();
    }

    void submitPrimitives(uint32_t elements) {
        s_frame.setIsIndexed(false);
        s_frame.setNumberOfElements(elements);
        s_frame.submitCurrentDrawCall();
        s_frame.beginDrawCall();
    }

} // namespace Miren

} // namespace Panda
