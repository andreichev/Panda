//
// Created by Admin on 10.03.2022.
//

#include "pndpch.hpp"
#include "Miren.hpp"

#include "Platform/RendererImpl/OpenGL/RendererOpenGL.hpp"

namespace Panda {

namespace Miren {

    // TODO: - Create context
    bool needToIntialize = false;
    RendererI *s_context = nullptr;
    Frame s_frame = Frame();
    constexpr uint32_t maxHandles = 1000;

    MirenHandleAllocator s_shadersHandleAlloc(maxHandles);
    MirenHandleAllocator s_texturesHandleAlloc(maxHandles);
    MirenHandleAllocator s_vertexLayoutsHandleAlloc(maxHandles);
    MirenHandleAllocator s_vertexBuffersHandleAlloc(maxHandles);
    MirenHandleAllocator s_indexBuffersHandleAlloc(maxHandles);

    CommandQueue s_commandQueue;
    Semaphore rendererSemaphore;

    void initialize() {
        needToIntialize = true;
    }

    void terminate() {
        delete s_context;
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

    void rendererExecuteCommands();

    void renderFrame() {
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
            s_context->submit(draw);
            s_frame.free(draw);
        }
        s_context->flip();
        s_frame.beginDrawCall();
        renderSemaphorePost();
    }

    void rendererExecuteCommands() {
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

    void setScissorRect(UIRect rect) {
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
