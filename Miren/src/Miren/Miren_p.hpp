#pragma once

#include "CommandBuffer/RendererCommand.hpp"
#include "CommandBuffer/Frame.hpp"
#include "CommandBuffer/RenderDraw.hpp"
#include "Miren/Miren.hpp"
#include "MirenHandleAllocator.hpp"
#include "MirenStates.hpp"
#include "Platform/RendererImpl/OpenGL/RendererOpenGL.hpp"
#include "RendererI.hpp"

#include <Foundation/PlatformDetection.hpp>
#include <Foundation/Foundation.hpp>
#include <Foundation/Logger.hpp>
#include <Foundation/Semaphore.hpp>
#include <Foundation/Thread.hpp>

namespace Miren {

struct Context {
    Context()
        : m_renderer(nullptr)
        , m_shadersHandleAlloc(MAX_SHADERS)
        , m_texturesHandleAlloc(MAX_TEXTURES)
        , m_vertexLayoutsHandleAlloc(MAX_BUFFER_LAYOUTS)
        , m_vertexBuffersHandleAlloc(MAX_VERTEX_BUFFERS)
        , m_indexBuffersHandleAlloc(MAX_INDEX_BUFFERS)
        // TODO: - Add reusable vertex buffers
        // , m_dynamicVertexBuffersHandleAlloc(MAX_DYNAMIC_VERTEX_BUFFERS)
        // , m_dynamicIndexBuffersHandleAlloc(MAX_DYNAMIC_INDEX_BUFFERS)
        , m_commandQueue(300000)
        , m_rendererSemaphore() {
        m_render = &m_frame[0];
        m_submit = &m_frame[1];
    }

    void init() {
        MIREN_LOG("MIREN INIT BEGIN");
        m_rendererSemaphore.post();
#ifdef PLATFORM_DESKTOP
        m_thread.init(renderThread, nullptr, 0, "Render thread");
#endif
        Foundation::CommandBuffer::Command cmd(RendererCommandType::RendererInit);
        m_commandQueue.write(cmd);
        m_submit->m_transientVb = createTransientVertexBuffer(TRANSIENT_VERTEX_BUFFER_SIZE);
        m_submit->m_transientIb = createTransientIndexBuffer(TRANSIENT_INDEX_BUFFER_SIZE);
        frame();
        m_submit->m_transientVb = createTransientVertexBuffer(TRANSIENT_VERTEX_BUFFER_SIZE);
        m_submit->m_transientIb = createTransientIndexBuffer(TRANSIENT_INDEX_BUFFER_SIZE);
        frame();
        MIREN_LOG("MIREN INIT END");
    }

    // Должно быть вызвано из главного потока и не из цикла обновления.
    void shutdown() {
        MIREN_LOG("MIREN SHUTDOWN BEGIN");
        destroyTransientVertexBuffer(m_submit->m_transientVb);
        destroyTransientIndexBuffer(m_submit->m_transientIb);
        frame();
        destroyTransientVertexBuffer(m_submit->m_transientVb);
        destroyTransientIndexBuffer(m_submit->m_transientIb);
        frame();
        Foundation::CommandBuffer::Command cmd(RendererCommandType::RendererShutdown);
        m_commandQueue.write(cmd);
        m_rendererSemaphore.post();
        m_thread.shutdown();
        MIREN_LOG("MIREN SHUTDOWN END");
    }

    static int32_t renderThread(Foundation::Thread *_thread, void *_userData) {
        MIREN_LOG("RENDER THREAD BEGIN");
        while (Miren::renderFrame())
            ;
        MIREN_LOG("RENDER THREAD END");
        return 0;
    }

    bool rendererExecuteCommands() {
        m_commandQueue.finishWriting();
        Foundation::CommandBuffer::Command *command;
        if (m_renderer == nullptr) {
            command = m_commandQueue.read();
            if (command != nullptr) {
                CMDBUF_LOG("RENDERER INIT COMMAND");
                ASSERT(command->type == RendererCommandType::RendererInit, "First command should be RendererInit");
                m_renderer = NEW(Foundation::getAllocator(), RendererOpenGL);
            }
        }

        if (m_renderer == nullptr) {
            m_commandQueue.reset();
            return true;
        }

        while ((command = m_commandQueue.read()) != nullptr) {
            switch (command->type) {
                case RendererCommandType::RendererInit: {
                    break;
                }
                case RendererCommandType::RendererShutdown: {
                    CMDBUF_LOG("RENDERER SHUTDOWN COMMAND");
                    DELETE(Foundation::getAllocator(), m_renderer);
                    m_renderer = nullptr;
                    return false;
                }
                case RendererCommandType::CreateShader: {
                    CMDBUF_LOG("CREATE SHADER COMMAND");
                    const CreateShaderCommand *cmd = static_cast<const CreateShaderCommand *>(command);
                    m_renderer->createShader(cmd->handle, cmd->vertexPath, cmd->fragmentPath);
                    break;
                }
                case RendererCommandType::DestroyShader: {
                    CMDBUF_LOG("DESTROY SHADER COMMAND");
                    const DeleteShaderCommand *cmd = static_cast<const DeleteShaderCommand *>(command);
                    m_renderer->deleteShader(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateTextureFromFile: {
                    CMDBUF_LOG("CREATE TEXTURE FROM FILE COMMAND");
                    const CreateTextureFromFileCommand *cmd = static_cast<const CreateTextureFromFileCommand *>(command);
                    m_renderer->createTextureFromFile(cmd->handle, cmd->path);
                    break;
                }
                case RendererCommandType::CreateRGBATextureFromPixelsBuffer: {
                    CMDBUF_LOG("CREATE TEXTURE FROM PIXELS COMMAND");
                    const CreateRGBATextureFromPixelsCommand *cmd = static_cast<const CreateRGBATextureFromPixelsCommand *>(command);
                    m_renderer->createRGBATextureFromPixels(cmd->handle, cmd->pixels, cmd->width, cmd->height);
                    break;
                }
                case RendererCommandType::DestroyTexture: {
                    CMDBUF_LOG("DESTROY TEXTURE COMMAND");
                    const DeleteTextureCommand *cmd = static_cast<const DeleteTextureCommand *>(command);
                    m_renderer->deleteTexture(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateIndexBuffer: {
                    CMDBUF_LOG("CREATE INDEX BUFFER COMMAND");
                    const CreateIndexBufferCommand *cmd = static_cast<const CreateIndexBufferCommand *>(command);
                    m_renderer->createIndexBuffer(cmd->handle, cmd->indices, cmd->elementType, cmd->count);
                    break;
                }
                case RendererCommandType::CreateDynamicIndexBuffer: {
                    CMDBUF_LOG("CREATE DYNAMIC INDEX BUFFER COMMAND");
                    const CreateDynamicIndexBufferCommand *cmd = static_cast<const CreateDynamicIndexBufferCommand *>(command);
                    m_renderer->createDynamicIndexBuffer(cmd->handle, cmd->indices, cmd->elementType, cmd->count);
                    break;
                }
                case RendererCommandType::UpdateDynamicIndexBuffer: {
                    CMDBUF_LOG("UPDATE DYNAMIC INDEX BUFFER COMMAND");
                    const UpdateDynamicIndexBufferCommand *cmd = static_cast<const UpdateDynamicIndexBufferCommand *>(command);
                    m_renderer->updateDynamicIndexBuffer(cmd->handle, cmd->indices, cmd->count);
                    break;
                }
                case RendererCommandType::DestroyIndexBuffer: {
                    CMDBUF_LOG("DESTROY INDEX BUFFER COMMAND");
                    const DeleteIndexBufferCommand *cmd = static_cast<const DeleteIndexBufferCommand *>(command);
                    m_renderer->deleteIndexBuffer(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateVertexBuffer: {
                    CMDBUF_LOG("CREATE VERTEX BUFFER COMMAND");
                    const CreateVertexBufferCommand *cmd = static_cast<const CreateVertexBufferCommand *>(command);
                    m_renderer->createVertexBuffer(cmd->handle, cmd->data, cmd->size, cmd->layoutHandle);
                    break;
                }
                case RendererCommandType::CreateDynamicVertexBuffer: {
                    CMDBUF_LOG("CREATE DYNAMIC VERTEX BUFFER COMMAND");
                    const CreateDynamicVertexBufferCommand *cmd = static_cast<const CreateDynamicVertexBufferCommand *>(command);
                    m_renderer->createDynamicVertexBuffer(cmd->handle, cmd->data, cmd->size, cmd->layoutHandle);
                    break;
                }
                case RendererCommandType::UpdateDynamicVertexBuffer: {
                    CMDBUF_LOG("UPDATE DYNAMIC VERTEX BUFFER COMMAND");
                    const UpdateDynamicVertexBufferCommand *cmd = static_cast<const UpdateDynamicVertexBufferCommand *>(command);
                    m_renderer->updateDynamicVertexBuffer(cmd->handle, cmd->data, cmd->size);
                    break;
                }
                case RendererCommandType::DestroyVertexBuffer: {
                    CMDBUF_LOG("DESTROY VERTEX BUFFER COMMAND");
                    const DeleteVertexBufferCommand *cmd = static_cast<const DeleteVertexBufferCommand *>(command);
                    m_renderer->deleteVertexBuffer(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateVertexLayout: {
                    CMDBUF_LOG("CREATE VERTEX LAYOUT COMMAND");
                    const CreateVertexLayoutCommand *cmd = static_cast<const CreateVertexLayoutCommand *>(command);
                    m_renderer->createVertexLayout(cmd->handle, cmd->data);
                    break;
                }
                case RendererCommandType::DestroyVertexLayout: {
                    CMDBUF_LOG("DESTROY VERTEX LAYOYT COMMAND");
                    const DeleteVertexLayoutCommand *cmd = static_cast<const DeleteVertexLayoutCommand *>(command);
                    m_renderer->deleteVertexLayout(cmd->handle);
                    break;
                }
            }
        }
        m_commandQueue.reset();
        return true;
    }

    bool renderFrame() {
        m_rendererSemaphore.wait();
        MIREN_LOG("RENDER FRAME BEGIN");
        if (rendererExecuteCommands() == false) {
            MIREN_LOG("RENDER FRAME END");
            m_rendererSemaphore.post();
            return false;
        }
        ASSERT(m_renderer != nullptr, "RENDERER IS NULL");
        if (m_render->getDrawCallsCount() == 0) {
            MIREN_LOG("RENDER FRAME END");
            m_rendererSemaphore.post();
            return true;
        }
        m_renderer->clear();
        m_renderer->submit(m_render);
        m_renderer->flip();
        MIREN_LOG("RENDER FRAME END");
        m_rendererSemaphore.post();
        return true;
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

    VertexBufferHandle createDynamicVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle = MIREN_INVALID_HANDLE) {
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

    void destroyTransientVertexBuffer(TransientVertexBuffer &tvb) {
        deleteVertexBuffer(tvb.handle);
        ALIGNED_FREE(Foundation::getAllocator(), tvb.data, 16);
    }

    void destroyTransientIndexBuffer(TransientIndexBuffer &tib) {
        deleteIndexBuffer(tib.handle);
        ALIGNED_FREE(Foundation::getAllocator(), tib.data, 16);
    }

    TransientIndexBuffer createTransientIndexBuffer(uint32_t size) {
        TransientIndexBuffer tib;
        tib.data = (uint8_t *)ALIGNED_ALLOC(Foundation::getAllocator(), size, 16);
        tib.size = size;
        tib.startIndex = 0;
        tib.handle = createDynamicIndexBuffer(nullptr, BufferElementType::UnsignedShort, size / 2);
        tib.elementType = BufferElementType::UnsignedShort;
        return tib;
    }

    TransientVertexBuffer createTransientVertexBuffer(uint32_t size) {
        TransientVertexBuffer tvb;
        tvb.data = (uint8_t *)ALIGNED_ALLOC(Foundation::getAllocator(), size, 16);
        tvb.size = size;
        tvb.startVertex = 0;
        tvb.handle = createDynamicVertexBuffer(nullptr, size);
        return tvb;
    }

    void allocTransientVertexBuffer(TransientVertexBuffer *buffer, uint32_t size) {
        uint32_t transientVBOffset = m_submit->m_transientVbSize;
        buffer->data = &m_submit->m_transientVb.data[transientVBOffset];
        m_submit->m_transientVbSize += size;
        buffer->size = size;
        buffer->startVertex = transientVBOffset;
        buffer->handle = m_submit->m_transientVb.handle;
    }

    void allocTransientIndexBuffer(TransientIndexBuffer *buffer, uint32_t count, BufferElementType elementType) {
        uint32_t transientIBOffset = m_submit->m_transientIbSize;
        uint32_t size = count * VertexBufferElement::getSizeOfType(elementType);
        buffer->data = &m_submit->m_transientIb.data[transientIBOffset];
        m_submit->m_transientIbSize += size;
        buffer->size = size;
        buffer->startIndex = transientIBOffset;
        buffer->elementType = elementType;
        buffer->handle = m_submit->m_transientIb.handle;
    }

    void setState(uint32_t state) {
        m_submit->setState(state);
    }

    void setScissorRect(Rect rect) {
        m_submit->setScissorRect(rect);
    }

    void setVertexBuffer(VertexBufferHandle handle, intptr_t offset) {
        m_submit->setVertexBuffer(handle, offset);
    }

    void setIndexBuffer(IndexBufferHandle handle, intptr_t offset, size_t count) {
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

    void setVertexLayout(VertexLayoutHandle handle) {
        m_submit->setVertexLayout(handle);
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

    // TODO: - Add reusable buffers:
    // DynamicIndexBuffer m_dynamicIndexBuffers[MAX_DYNAMIC_INDEX_BUFFERS];
    // DynamicVertexBuffer m_dynamicVertexBuffers[MAX_DYNAMIC_VERTEX_BUFFERS];

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
