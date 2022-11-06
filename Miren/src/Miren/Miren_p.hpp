#pragma once

#include "CommandBuffer/RendererCommand.hpp"
#include "CommandBuffer/Frame.hpp"
#include "CommandBuffer/RenderDraw.hpp"
#include "Miren/Miren.hpp"
#include "MirenHandleAllocator.hpp"
#include "MirenStates.hpp"
#include "Platform/RendererImpl/OpenGL/RendererOpenGL.hpp"
#include "RendererI.hpp"

#include <Foundation/Foundation.hpp>
#include <Foundation/Logger.hpp>
#include <Foundation/Semaphore.hpp>
#include <Foundation/Thread.hpp>

namespace Miren {

uint32_t maxHandles = 1000;

struct Context {
    Context()
        : m_renderer(nullptr)
        , m_shadersHandleAlloc(maxHandles)
        , m_texturesHandleAlloc(maxHandles)
        , m_vertexLayoutsHandleAlloc(maxHandles)
        , m_vertexBuffersHandleAlloc(maxHandles)
        , m_indexBuffersHandleAlloc(maxHandles)
        , m_commandQueue(300000)
        , m_rendererSemaphore() {
        m_render = &m_frame[0];
        m_submit = &m_frame[1];
    }

    void init() {
        m_rendererSemaphore.post();
        m_thread.init(renderThread, nullptr, 0, "Render thread");
        Foundation::CommandBuffer::Command cmd(RendererCommandType::RendererInit);
        m_commandQueue.write(cmd);
        // m_submit->m_transientVb = createTransientVertexBuffer(TRANSIENT_VERTEX_BUFFER_SIZE, VertexBufferLayoutData());
        // m_submit->m_transientIb = createTransientIndexBuffer(TRANSIENT_INDEX_BUFFER_SIZE);
    }

    void shutdown() {
        DELETE(Foundation::getAllocator(), m_renderer);
        m_thread.shutdown();
    }

    static int32_t renderThread(Foundation::Thread *_thread, void *_userData) {
        while (true) {
            Miren::renderFrame();
        }
        return 0;
    }

    void rendererExecuteCommands() {
        m_commandQueue.finishWriting();
        Foundation::CommandBuffer::Command *command;
        if (m_renderer == nullptr) {
            command = m_commandQueue.read();
            if (command != nullptr) {
                ASSERT(command->type == RendererCommandType::RendererInit, "First command in command buffer should be RendererInit");
                m_renderer = NEW(Foundation::getAllocator(), RendererOpenGL);
            }
        }

        if (m_renderer == nullptr) {
            m_commandQueue.reset();
            return;
        }

        while ((command = m_commandQueue.read()) != nullptr) {
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
        }
        m_commandQueue.reset();
    }

    void renderFrame() {
        m_rendererSemaphore.wait();
        if (m_render->getDrawCallsCount() == 0) {
            m_rendererSemaphore.post();
            return;
        }
        rendererExecuteCommands();
        if (m_renderer == nullptr) {
            m_rendererSemaphore.post();
            return;
        }
        m_renderer->clear();

        for (int i = 0; i < m_render->getDrawCallsCount(); i++) {
            RenderDraw &draw = m_render->getDrawCalls()[i];
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
        m_rendererSemaphore.post();
    }

    ShaderHandle createShader(const char *vertexPath, const char *fragmentPath) {
        ShaderHandle handle = m_shadersHandleAlloc.alloc();
        CreateShaderCommand cmd(handle, vertexPath, fragmentPath);
        m_commandQueue.write(cmd);
        return handle;
    }

    void deleteShader(ShaderHandle handle) {
        m_shadersHandleAlloc.free(handle);
        DeleteShaderCommand cmd(handle);
        m_commandQueue.write(cmd);
    }

    TextureHandle createTextureFromFile(const char *path) {
        TextureHandle handle = m_texturesHandleAlloc.alloc();
        CreateTextureFromFileCommand cmd(handle, path);
        m_commandQueue.write(cmd);
        return handle;
    }

    TextureHandle createTextureFromPixels(void *pixels, int width, int height) {
        TextureHandle handle = m_texturesHandleAlloc.alloc();
        CreateRGBATextureFromPixelsCommand cmd(handle, pixels, width, height);
        m_commandQueue.write(cmd);
        return handle;
    }

    void deleteTexture(TextureHandle handle) {
        m_texturesHandleAlloc.free(handle);
        DeleteTextureCommand cmd(handle);
        m_commandQueue.write(cmd);
    }

    IndexBufferHandle createIndexBuffer(void *indices, BufferElementType elementType, size_t count) {
        IndexBufferHandle handle = m_indexBuffersHandleAlloc.alloc();
        CreateIndexBufferCommand cmd(handle, indices, elementType, count);
        m_commandQueue.write(cmd);
        return handle;
    }

    IndexBufferHandle createDynamicIndexBuffer(void *indices, BufferElementType elementType, size_t count) {
        IndexBufferHandle handle = m_indexBuffersHandleAlloc.alloc();
        CreateDynamicIndexBufferCommand cmd(handle, indices, elementType, count);
        m_commandQueue.write(cmd);
        return handle;
    }

    void updateDynamicIndexBuffer(IndexBufferHandle handle, void *indices, size_t count) {
        UpdateDynamicIndexBufferCommand cmd(handle, indices, count);
        m_commandQueue.write(cmd);
    }

    void deleteIndexBuffer(IndexBufferHandle handle) {
        m_indexBuffersHandleAlloc.free(handle);
        DeleteIndexBufferCommand cmd(handle);
        m_commandQueue.write(cmd);
    }

    VertexBufferHandle createVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
        VertexBufferHandle handle = m_vertexBuffersHandleAlloc.alloc();
        CreateVertexBufferCommand cmd(handle, data, size, layoutHandle);
        m_commandQueue.write(cmd);
        return handle;
    }

    VertexBufferHandle createDynamicVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
        VertexBufferHandle handle = m_vertexBuffersHandleAlloc.alloc();
        CreateDynamicVertexBufferCommand cmd(handle, data, size, layoutHandle);
        m_commandQueue.write(cmd);
        return handle;
    }

    void updateDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size) {
        UpdateDynamicVertexBufferCommand cmd(handle, data, size);
        m_commandQueue.write(cmd);
    }

    void deleteVertexBuffer(VertexBufferHandle handle) {
        m_vertexBuffersHandleAlloc.free(handle);
        DeleteVertexBufferCommand cmd(handle);
        m_commandQueue.write(cmd);
    }

    VertexLayoutHandle createVertexLayout(VertexBufferLayoutData data) {
        VertexLayoutHandle handle = m_vertexLayoutsHandleAlloc.alloc();
        CreateVertexLayoutCommand cmd(handle, data);
        m_commandQueue.write(cmd);
        return handle;
    }

    void deleteVertexLayout(VertexLayoutHandle handle) {
        m_vertexLayoutsHandleAlloc.free(handle);
        DeleteVertexLayoutCommand cmd(handle);
        m_commandQueue.write(cmd);
    }

    TransientIndexBuffer *createTransientIndexBuffer(uint32_t _size) {
        TransientIndexBuffer *tib = NULL;

        IndexBufferHandle handle = m_indexBuffersHandleAlloc.alloc();
        CreateDynamicIndexBufferCommand cmd(handle, nullptr, BufferElementType::UnsignedInt, _size / 4);
        m_commandQueue.write(cmd);

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

        CreateDynamicVertexBufferCommand cmd(handle, nullptr, size, layoutHandle);
        m_commandQueue.write(cmd);

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
        m_submit->setState(state);
    }

    void setScissorRect(Rect rect) {
        m_submit->setScissorRect(rect);
    }

    void setVertexBuffer(VertexBufferHandle handle) {
        m_submit->setVertexBuffer(handle);
    }

    void setIndexBuffer(IndexBufferHandle handle, void *offset, size_t count) {
        m_submit->setIndexBuffer(handle, offset, count);
    }

    void setShader(ShaderHandle handle) {
        m_submit->setShader(handle);
    }

    void setUniform(ShaderHandle handle, const char *name, void *value, UniformDataType type) {
        m_submit->setUniform(handle, name, value, type);
    }

    void setTexture(TextureHandle textureHandle, uint32_t slot) {
        m_submit->setTexture(textureHandle, slot);
    }

    void submit() {
        m_submit->submitCurrentDrawCall();
        m_submit->beginDrawCall();
    }

    void submitPrimitives(uint32_t elements) {
        m_submit->setIsIndexed(false);
        m_submit->setNumberOfElements(elements);
        m_submit->submitCurrentDrawCall();
        m_submit->beginDrawCall();
    }

    void swap() {
        Foundation::swap(m_render, m_submit);
    }

    void frame() {
        swap();
        m_submit->reset();
    }

private:
    Foundation::Thread m_thread;
    RendererI *m_renderer;
    Frame m_frame[2];
    Frame *m_render;
    Frame *m_submit;
    MirenHandleAllocator m_shadersHandleAlloc;
    MirenHandleAllocator m_texturesHandleAlloc;
    MirenHandleAllocator m_vertexLayoutsHandleAlloc;
    MirenHandleAllocator m_vertexBuffersHandleAlloc;
    MirenHandleAllocator m_indexBuffersHandleAlloc;
    Foundation::CommandBuffer m_commandQueue;

public:
    Foundation::Semaphore m_rendererSemaphore;
};

} // namespace Miren
