#pragma once

#include "Miren/Miren.hpp"
#include "MirenHandleAllocator.hpp"
#include "CommandBuffer/Frame.hpp"
#include "CommandBuffer/RenderDraw.hpp"
#include "CommandBuffer/CommandQueue.hpp"
#include "MirenStates.hpp"
#include "RendererI.hpp"
#include "Platform/RendererImpl/OpenGL/RendererOpenGL.hpp"

#include <Foundation/Foundation.hpp>
#include <Foundation/Thread.hpp>
#include <Foundation/Semaphore.hpp>
#include <Foundation/Semaphore.hpp>
#include <Foundation/Logger.hpp>

namespace Miren {

uint32_t maxHandles = 1000;

struct Context {
    Context()
        : m_renderer(nullptr)
        , m_frame()
        , m_shadersHandleAlloc(maxHandles)
        , m_texturesHandleAlloc(maxHandles)
        , m_vertexLayoutsHandleAlloc(maxHandles)
        , m_vertexBuffersHandleAlloc(maxHandles)
        , m_indexBuffersHandleAlloc(maxHandles)
        , m_commandQueue()
        , m_rendererSemaphore() {}

    void init() {
        m_thread.init(renderThread, nullptr, 0, "Render thread");
        m_commandQueue.post(new RendererCommand(RendererCommandType::RendererInit));
        m_frame.m_transientVb = createTransientVertexBuffer(TRANSIENT_VERTEX_BUFFER_SIZE, VertexBufferLayoutData());
        m_frame.m_transientIb = createTransientIndexBuffer(TRANSIENT_INDEX_BUFFER_SIZE);
    }

    void shutdown() {
        delete m_renderer;
        m_thread.shutdown();
    }

    static int32_t renderThread(Foundation::Thread *_thread, void *_userData) {
        while (true) {
            Miren::renderFrame();
        }
        return 0;
    }

    void rendererExecuteCommands() {
        const RendererCommand *command;
        if (m_renderer == nullptr) {
            command = m_commandQueue.poll();
            if (command != nullptr) {
                ASSERT(command->type == RendererCommandType::RendererInit, "First command in command buffer should be RendererInit");
                m_renderer = new RendererOpenGL();
                m_commandQueue.release(command);
            }
        }

        if (m_renderer == nullptr) {
            return;
        }

        while ((command = m_commandQueue.poll()) != nullptr) {
            switch (command->type) {
                case RendererCommandType::RendererInit: {
                    break;
                }
                case RendererCommandType::CreateShader: {
                    const CreateShaderCommand *cmd = static_cast<const CreateShaderCommand *>(command);
                    m_renderer->createShader(cmd->handle, cmd->vertexPath, cmd->fragmentPath);
                    break;
                }
                case RendererCommandType::DestroyShader: {
                    const DeleteShaderCommand *cmd = static_cast<const DeleteShaderCommand *>(command);
                    m_renderer->deleteShader(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateTextureFromFile: {
                    const CreateTextureFromFileCommand *cmd = static_cast<const CreateTextureFromFileCommand *>(command);
                    m_renderer->createTextureFromFile(cmd->handle, cmd->path);
                    break;
                }
                case RendererCommandType::CreateRGBATextureFromPixelsBuffer: {
                    const CreateRGBATextureFromPixelsCommand *cmd = static_cast<const CreateRGBATextureFromPixelsCommand *>(command);
                    m_renderer->createRGBATextureFromPixels(cmd->handle, cmd->pixels, cmd->width, cmd->height);
                    break;
                }
                case RendererCommandType::DestroyTexture: {
                    const DeleteTextureCommand *cmd = static_cast<const DeleteTextureCommand *>(command);
                    m_renderer->deleteTexture(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateIndexBuffer: {
                    const CreateIndexBufferCommand *cmd = static_cast<const CreateIndexBufferCommand *>(command);
                    m_renderer->createIndexBuffer(cmd->handle, cmd->indices, cmd->elementType, cmd->count);
                    break;
                }
                case RendererCommandType::CreateDynamicIndexBuffer: {
                    const CreateDynamicIndexBufferCommand *cmd = static_cast<const CreateDynamicIndexBufferCommand *>(command);
                    m_renderer->createDynamicIndexBuffer(cmd->handle, cmd->indices, cmd->elementType, cmd->count);
                    break;
                }
                case RendererCommandType::UpdateDynamicIndexBuffer: {
                    const UpdateDynamicIndexBufferCommand *cmd = static_cast<const UpdateDynamicIndexBufferCommand *>(command);
                    m_renderer->updateDynamicIndexBuffer(cmd->handle, cmd->indices, cmd->count);
                    break;
                }
                case RendererCommandType::DestroyIndexBuffer: {
                    const DeleteIndexBufferCommand *cmd = static_cast<const DeleteIndexBufferCommand *>(command);
                    m_renderer->deleteIndexBuffer(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateVertexBuffer: {
                    const CreateVertexBufferCommand *cmd = static_cast<const CreateVertexBufferCommand *>(command);
                    m_renderer->createVertexBuffer(cmd->handle, cmd->data, cmd->size, cmd->layoutHandle);
                    break;
                }
                case RendererCommandType::CreateDynamicVertexBuffer: {
                    const CreateDynamicVertexBufferCommand *cmd = static_cast<const CreateDynamicVertexBufferCommand *>(command);
                    m_renderer->createDynamicVertexBuffer(cmd->handle, cmd->data, cmd->size, cmd->layoutHandle);
                    break;
                }
                case RendererCommandType::UpdateDynamicVertexBuffer: {
                    const UpdateDynamicVertexBufferCommand *cmd = static_cast<const UpdateDynamicVertexBufferCommand *>(command);
                    m_renderer->updateDynamicVertexBuffer(cmd->handle, cmd->data, cmd->size);
                    break;
                }
                case RendererCommandType::DestroyVertexBuffer: {
                    const DeleteVertexBufferCommand *cmd = static_cast<const DeleteVertexBufferCommand *>(command);
                    m_renderer->deleteVertexBuffer(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateVertexLayout: {
                    const CreateVertexLayoutCommand *cmd = static_cast<const CreateVertexLayoutCommand *>(command);
                    m_renderer->createVertexLayout(cmd->handle, cmd->data);
                    break;
                }
                case RendererCommandType::DestroyVertexLayout: {
                    const DeleteVertexLayoutCommand *cmd = static_cast<const DeleteVertexLayoutCommand *>(command);
                    m_renderer->deleteVertexLayout(cmd->handle);
                    break;
                }
            }
            m_commandQueue.release(command);
        }
    }

    void renderFrame() {
        if (m_frame.getDrawCallsCount() == 0) {
            return;
        }
        rendererExecuteCommands();
        if (m_renderer == nullptr) {
            return;
        }
        m_renderer->clear();
        m_rendererSemaphore.wait();

        for (int i = 0; i < m_frame.getDrawCallsCount(); i++) {
            RenderDraw &draw = m_frame.getDrawCalls()[i];
            if (draw.m_isSubmitted == false) {
                continue;
            }
            for (size_t u = 0; u < draw.m_uniformsCount; u++) {
                Uniform &uniform = draw.m_uniformBuffer[u];
                m_renderer->setUniform(uniform);
            }
            for (size_t t = 0; t < draw.m_textureBindingsCount; t++) {
                TextureBinding &textureBinding = draw.m_textureBindings[t];
                m_renderer->setTexture(textureBinding.m_handle, textureBinding.m_slot);
            }
            m_renderer->submit(&draw);
        }
        m_renderer->flip();
        m_frame.reset();
        m_rendererSemaphore.post();
    }

    ShaderHandle createShader(const char *vertexPath, const char *fragmentPath) {
        ShaderHandle handle = m_shadersHandleAlloc.alloc();
        m_commandQueue.post(new CreateShaderCommand(handle, vertexPath, fragmentPath));
        return handle;
    }

    void deleteShader(ShaderHandle handle) {
        m_shadersHandleAlloc.free(handle);
        m_commandQueue.post(new DeleteShaderCommand(handle));
    }

    TextureHandle createTextureFromFile(const char *path) {
        TextureHandle handle = m_texturesHandleAlloc.alloc();
        m_commandQueue.post(new CreateTextureFromFileCommand(handle, path));
        return handle;
    }

    TextureHandle createTextureFromPixels(void *pixels, int width, int height) {
        TextureHandle handle = m_texturesHandleAlloc.alloc();
        m_commandQueue.post(new CreateRGBATextureFromPixelsCommand(handle, pixels, width, height));
        return handle;
    }

    void deleteTexture(TextureHandle handle) {
        m_texturesHandleAlloc.free(handle);
        m_commandQueue.post(new DeleteTextureCommand(handle));
    }

    IndexBufferHandle createIndexBuffer(void *indices, BufferElementType elementType, size_t count) {
        IndexBufferHandle handle = m_indexBuffersHandleAlloc.alloc();
        m_commandQueue.post(new CreateIndexBufferCommand(handle, indices, elementType, count));
        return handle;
    }

    IndexBufferHandle createDynamicIndexBuffer(void *indices, BufferElementType elementType, size_t count) {
        IndexBufferHandle handle = m_indexBuffersHandleAlloc.alloc();
        m_commandQueue.post(new CreateDynamicIndexBufferCommand(handle, indices, elementType, count));
        return handle;
    }

    void updateDynamicIndexBuffer(IndexBufferHandle handle, void *indices, size_t count) {
        m_commandQueue.post(new UpdateDynamicIndexBufferCommand(handle, indices, count));
    }

    void deleteIndexBuffer(IndexBufferHandle handle) {
        m_indexBuffersHandleAlloc.free(handle);
        m_commandQueue.post(new DeleteIndexBufferCommand(handle));
    }

    VertexBufferHandle createVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
        VertexBufferHandle handle = m_vertexBuffersHandleAlloc.alloc();
        m_commandQueue.post(new CreateVertexBufferCommand(handle, data, size, layoutHandle));
        return handle;
    }

    VertexBufferHandle createDynamicVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
        VertexBufferHandle handle = m_vertexBuffersHandleAlloc.alloc();
        m_commandQueue.post(new CreateDynamicVertexBufferCommand(handle, data, size, layoutHandle));
        return handle;
    }

    void updateDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size) {
        m_commandQueue.post(new UpdateDynamicVertexBufferCommand(handle, data, size));
    }

    void deleteVertexBuffer(VertexBufferHandle handle) {
        m_vertexBuffersHandleAlloc.free(handle);
        m_commandQueue.post(new DeleteVertexBufferCommand(handle));
    }

    VertexLayoutHandle createVertexLayout(VertexBufferLayoutData data) {
        VertexLayoutHandle handle = m_vertexLayoutsHandleAlloc.alloc();
        m_commandQueue.post(new CreateVertexLayoutCommand(handle, data));
        return handle;
    }

    void deleteVertexLayout(VertexLayoutHandle handle) {
        m_vertexLayoutsHandleAlloc.free(handle);
        m_commandQueue.post(new DeleteVertexLayoutCommand(handle));
    }

    TransientIndexBuffer *createTransientIndexBuffer(uint32_t _size) {
        TransientIndexBuffer *tib = NULL;

        IndexBufferHandle handle = m_indexBuffersHandleAlloc.alloc();
        RendererCommand *cmd = new CreateDynamicIndexBufferCommand(handle, nullptr, BufferElementType::UnsignedInt, _size / 4);
        m_commandQueue.post(cmd);

        const uint32_t size = sizeof(TransientIndexBuffer) + _size;
        tib = (TransientIndexBuffer *)ALLOC(Foundation::getAllocator(), size);
        tib->data = (uint8_t *)tib + sizeof(TransientIndexBuffer);
        tib->size = _size;
        tib->handle = handle;

        return tib;
    }

    TransientVertexBuffer *createTransientVertexBuffer(uint32_t size, VertexBufferLayoutData layout) {
        TransientVertexBuffer *tvb = nullptr;
        VertexBufferHandle handle = m_vertexBuffersHandleAlloc.alloc();

        uint16_t stride = 0;
        VertexLayoutHandle layoutHandle = createVertexLayout(layout);
        stride = layout.m_Stride;

        CreateDynamicVertexBufferCommand *cmd = new CreateDynamicVertexBufferCommand(handle, nullptr, size, layoutHandle);
        m_commandQueue.post(cmd);

        tvb = (TransientVertexBuffer *)ALLOC(Foundation::getAllocator(), size);
        tvb->data = (uint8_t *)tvb + sizeof(TransientVertexBuffer);
        tvb->size = size;
        tvb->startVertex = 0;
        tvb->stride = stride;
        tvb->handle = handle;
        tvb->layoutHandle = layoutHandle;

        return tvb;
    }

    void setState(uint32_t state) {
        m_frame.setState(state);
    }

    void setScissorRect(Rect rect) {
        m_frame.setScissorRect(rect);
    }

    void setVertexBuffer(VertexBufferHandle handle) {
        m_frame.setVertexBuffer(handle);
    }

    void setIndexBuffer(IndexBufferHandle handle, void *offset, size_t count) {
        m_frame.setIndexBuffer(handle, offset, count);
    }

    void setShader(ShaderHandle handle) {
        m_frame.setShader(handle);
    }

    void setUniform(ShaderHandle handle, const char *name, void *value, UniformDataType type) {
        m_frame.setUniform(handle, name, value, type);
    }

    void setTexture(TextureHandle textureHandle, uint32_t slot) {
        m_frame.setTexture(textureHandle, slot);
    }

    void submit() {
        m_frame.submitCurrentDrawCall();
        m_frame.beginDrawCall();
    }

    void submitPrimitives(uint32_t elements) {
        m_frame.setIsIndexed(false);
        m_frame.setNumberOfElements(elements);
        m_frame.submitCurrentDrawCall();
        m_frame.beginDrawCall();
    }

private:
    Foundation::Thread m_thread;
    RendererI *m_renderer;
    Frame m_frame;
    MirenHandleAllocator m_shadersHandleAlloc;
    MirenHandleAllocator m_texturesHandleAlloc;
    MirenHandleAllocator m_vertexLayoutsHandleAlloc;
    MirenHandleAllocator m_vertexBuffersHandleAlloc;
    MirenHandleAllocator m_indexBuffersHandleAlloc;
    CommandQueue m_commandQueue;
    Foundation::Semaphore m_rendererSemaphore;
};

} // namespace Miren
