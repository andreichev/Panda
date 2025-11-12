#pragma once

#include "Encoder/RendererCommand.hpp"
#include "Encoder/Frame.hpp"
#include "Encoder/View.hpp"
#include "Encoder/RenderPass.hpp"
#include "HandleAllocator.hpp"
#include "Miren/MirenStates.hpp"
#include "Platform/RendererImpl/OpenGL/RendererOpenGL.hpp"
#include "RendererI.hpp"

#include <Foundation/PlatformDetection.hpp>
#include <Foundation/Foundation.hpp>
#include <Foundation/Logger.hpp>
#include <Foundation/Semaphore.hpp>
#include <Foundation/Thread.hpp>

namespace Miren {

struct Context {
    // Main thread
    Context()
        : m_renderer(nullptr)
        , m_frameNumber(0)
        , m_frameBuffersHandleAlloc(MAX_FRAME_BUFFERS)
        , m_shadersHandleAlloc(MAX_SHADERS)
        , m_texturesHandleAlloc(MAX_TEXTURES)
        , m_vertexLayoutsHandleAlloc(MAX_BUFFER_LAYOUTS)
        , m_vertexBuffersHandleAlloc(MAX_VERTEX_BUFFERS)
        , m_indexBuffersHandleAlloc(MAX_INDEX_BUFFERS)
        , m_rendererSemaphore("Render semaphore")
        , m_apiSemaphore("Api semaphore") {
        m_render = &m_frame[0];
        m_submit = &m_frame[1];
    }

    // Main thread
    void init(Fern::GraphicsContext *ctx) {
        m_ctx = ctx;
        // Вызвано из главного потока: можно стартовать поток отрисовки.
#ifdef PLATFORM_DESKTOP
        m_thread.init(renderThreadFunc, nullptr, 0, "Render thread");
#endif
        RendererInitCommand cmd(ctx);
        m_render->getPreCommandQueue().write(cmd);
        m_apiSemaphore.post();
        frame();
        m_submit->m_transientVb = createTransientVertexBuffer(TRANSIENT_VERTEX_BUFFER_SIZE);
        m_submit->m_transientIb = createTransientIndexBuffer(TRANSIENT_INDEX_BUFFER_SIZE);
        frame();
        m_submit->m_transientVb = createTransientVertexBuffer(TRANSIENT_VERTEX_BUFFER_SIZE);
        m_submit->m_transientIb = createTransientIndexBuffer(TRANSIENT_INDEX_BUFFER_SIZE);
        frame();
    }

    // Main thread
    void shutdown() {
        destroyTransientVertexBuffer(m_submit->m_transientVb);
        destroyTransientIndexBuffer(m_submit->m_transientIb);
        frame();
        destroyTransientVertexBuffer(m_submit->m_transientVb);
        destroyTransientIndexBuffer(m_submit->m_transientIb);
        frame();
        Foundation::CommandBuffer::Command cmd(RendererCommandType::RendererShutdown);
        m_submit->getPostCommandQueue().write(cmd);
        frame();
        m_thread.shutdown();
    }

    static int32_t renderThreadFunc(Foundation::Thread *_thread, void *_userData) {
        MIREN_LOG("RENDER THREAD BEGIN");
        while (Miren::renderFrame())
            ;
        MIREN_LOG("RENDER THREAD END");
        return 0;
    }

    void rendererExecuteCommands(Foundation::CommandBuffer &commandBuffer) {
        Foundation::CommandBuffer::Command *command;
        while ((command = commandBuffer.read()) != nullptr) {
            CMDBUF_LOG("COMMAND: %d", command->type);
            switch (command->type) {
                case RendererCommandType::RendererInit: {
                    break;
                }
                case RendererCommandType::RendererShutdown: {
                    CMDBUF_LOG("RENDERER SHUTDOWN COMMAND");
                    F_DELETE(Foundation::getAllocator(), m_renderer);
                    m_renderer = nullptr;
                    return;
                }
                case RendererCommandType::CreateFrameBuffer: {
                    CMDBUF_LOG("CREATE FRAME BUFFER COMMAND");
                    const CreateFrameBufferCommand *cmd =
                        static_cast<const CreateFrameBufferCommand *>(command);
                    m_renderer->createFrameBuffer(cmd->handle, cmd->spec);
                    break;
                }
                case RendererCommandType::DestroyFrameBuffer: {
                    CMDBUF_LOG("DESTROY FRAME BUFFER COMMAND");
                    const DeleteFrameBufferCommand *cmd =
                        static_cast<const DeleteFrameBufferCommand *>(command);
                    m_renderer->deleteFrameBuffer(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateProgram: {
                    CMDBUF_LOG("CREATE SHADER COMMAND");
                    const CreateProgramCommand *cmd =
                        static_cast<const CreateProgramCommand *>(command);
                    m_renderer->createProgram(cmd->handle, cmd->create);
                    break;
                }
                case RendererCommandType::DestroyProgram: {
                    CMDBUF_LOG("DESTROY SHADER COMMAND");
                    const DeleteProgramCommand *cmd =
                        static_cast<const DeleteProgramCommand *>(command);
                    m_renderer->deleteShader(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateTexture: {
                    CMDBUF_LOG("CREATE TEXTURE COMMAND");
                    const CreateTextureCommand *cmd =
                        static_cast<const CreateTextureCommand *>(command);
                    m_renderer->createTexture(cmd->handle, cmd->create);
                    break;
                }
                case RendererCommandType::ResizeTexture: {
                    CMDBUF_LOG("RESIZE TEXTURE COMMAND");
                    const ResizeTextureCommand *cmd =
                        static_cast<const ResizeTextureCommand *>(command);
                    m_renderer->resizeTexture(cmd->handle, cmd->width, cmd->height);
                    break;
                }
                case RendererCommandType::UpdateTexture: {
                    CMDBUF_LOG("UPDATE TEXTURE COMMAND");
                    const UpdateTextureCommand *cmd =
                        static_cast<const UpdateTextureCommand *>(command);
                    m_renderer->updateTexture(cmd->handle, cmd->mem);
                    break;
                }
                case RendererCommandType::DestroyTexture: {
                    CMDBUF_LOG("DESTROY TEXTURE COMMAND");
                    const DeleteTextureCommand *cmd =
                        static_cast<const DeleteTextureCommand *>(command);
                    m_renderer->deleteTexture(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateIndexBuffer: {
                    CMDBUF_LOG("CREATE INDEX BUFFER COMMAND");
                    const CreateIndexBufferCommand *cmd =
                        static_cast<const CreateIndexBufferCommand *>(command);
                    m_renderer->createIndexBuffer(
                        cmd->handle, cmd->indices, cmd->elementType, cmd->count
                    );
                    break;
                }
                case RendererCommandType::CreateDynamicIndexBuffer: {
                    CMDBUF_LOG("CREATE DYNAMIC INDEX BUFFER COMMAND");
                    const CreateDynamicIndexBufferCommand *cmd =
                        static_cast<const CreateDynamicIndexBufferCommand *>(command);
                    m_renderer->createDynamicIndexBuffer(
                        cmd->handle, cmd->indices, cmd->elementType, cmd->count
                    );
                    break;
                }
                case RendererCommandType::UpdateDynamicIndexBuffer: {
                    CMDBUF_LOG("UPDATE DYNAMIC INDEX BUFFER COMMAND");
                    const UpdateDynamicIndexBufferCommand *cmd =
                        static_cast<const UpdateDynamicIndexBufferCommand *>(command);
                    m_renderer->updateDynamicIndexBuffer(cmd->handle, cmd->indices, cmd->count);
                    break;
                }
                case RendererCommandType::DestroyIndexBuffer: {
                    CMDBUF_LOG("DESTROY INDEX BUFFER COMMAND");
                    const DeleteIndexBufferCommand *cmd =
                        static_cast<const DeleteIndexBufferCommand *>(command);
                    m_renderer->deleteIndexBuffer(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateVertexBuffer: {
                    CMDBUF_LOG("CREATE VERTEX BUFFER COMMAND");
                    const CreateVertexBufferCommand *cmd =
                        static_cast<const CreateVertexBufferCommand *>(command);
                    m_renderer->createVertexBuffer(
                        cmd->handle, cmd->data, cmd->size, cmd->layoutHandle
                    );
                    break;
                }
                case RendererCommandType::CreateDynamicVertexBuffer: {
                    CMDBUF_LOG("CREATE DYNAMIC VERTEX BUFFER COMMAND");
                    const CreateDynamicVertexBufferCommand *cmd =
                        static_cast<const CreateDynamicVertexBufferCommand *>(command);
                    m_renderer->createDynamicVertexBuffer(
                        cmd->handle, cmd->data, cmd->size, cmd->layoutHandle
                    );
                    break;
                }
                case RendererCommandType::UpdateDynamicVertexBuffer: {
                    CMDBUF_LOG("UPDATE DYNAMIC VERTEX BUFFER COMMAND");
                    const UpdateDynamicVertexBufferCommand *cmd =
                        static_cast<const UpdateDynamicVertexBufferCommand *>(command);
                    m_renderer->updateDynamicVertexBuffer(cmd->handle, cmd->data, cmd->size);
                    break;
                }
                case RendererCommandType::DestroyVertexBuffer: {
                    CMDBUF_LOG("DESTROY VERTEX BUFFER COMMAND");
                    const DeleteVertexBufferCommand *cmd =
                        static_cast<const DeleteVertexBufferCommand *>(command);
                    m_renderer->deleteVertexBuffer(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateVertexLayout: {
                    CMDBUF_LOG("CREATE VERTEX LAYOUT COMMAND");
                    const CreateVertexLayoutCommand *cmd =
                        static_cast<const CreateVertexLayoutCommand *>(command);
                    m_renderer->createVertexLayout(cmd->handle, cmd->data);
                    break;
                }
                case RendererCommandType::DestroyVertexLayout: {
                    CMDBUF_LOG("DESTROY VERTEX LAYOUT COMMAND");
                    const DeleteVertexLayoutCommand *cmd =
                        static_cast<const DeleteVertexLayoutCommand *>(command);
                    m_renderer->deleteVertexLayout(cmd->handle);
                    break;
                }
                case RendererCommandType::ReadTexture: {
                    CMDBUF_LOG("READ TEXTURE COMMAND");
                    const ReadTextureCommand *cmd =
                        static_cast<const ReadTextureCommand *>(command);
                    m_renderer->readTexture(cmd->handle, cmd->data);
                    break;
                }
                case RendererCommandType::ReadFrameBuffer: {
                    CMDBUF_LOG("READ FRAME BUFFER COMMAND");
                    const ReadFrameBufferCommand *cmd =
                        static_cast<const ReadFrameBufferCommand *>(command);
                    m_renderer->readFrameBuffer(
                        cmd->handle,
                        cmd->attachIndex,
                        cmd->x,
                        cmd->y,
                        cmd->width,
                        cmd->height,
                        cmd->data
                    );
                    break;
                }
                default: {
                    LOG_CRITICAL("UNKNOWN COMMAND");
                }
            }
        }
    }

    void checkIfHasInitCommand() {
        Foundation::CommandBuffer::Command *command = m_render->getPreCommandQueue().read();
        if (command != nullptr) {
            CMDBUF_LOG("RENDERER INIT COMMAND");
            PND_ASSERT(
                command->type == RendererCommandType::RendererInit,
                "First command should be RendererInit"
            );
            m_ctx->setCurrent();
            // TODO: Add other renderers (metal, directx, vulkan, ...)
            m_renderer = F_NEW(Foundation::getAllocator(), RendererOpenGL)(m_ctx);
            MIREN_LOG("RENDERER CREATED");
        }
    }

    bool renderFrame() {
        m_apiSemaphore.wait();
        MIREN_LOG("RENDER FRAME BEGIN");
        m_ctx->setCurrent();
        if (m_renderer == nullptr) { checkIfHasInitCommand(); }
        if (m_renderer == nullptr) {
            m_render->getPreCommandQueue().reset();
            m_render->getPostCommandQueue().reset();
            m_rendererSemaphore.post();
            return true;
        }
        rendererExecuteCommands(m_render->getPreCommandQueue());
        if (m_render->m_drawCallsCount != 0) {
            m_render->sort();
            m_renderer->submit(m_render);
            m_ctx->swapBuffers();
        }
        rendererExecuteCommands(m_render->getPostCommandQueue());
        MIREN_LOG("RENDER FRAME END");
        m_rendererSemaphore.post();
        return m_renderer != nullptr;
    }

    void freeAllHandles(Frame *frame) {
        frame->free(&m_frameBuffersHandleAlloc);
        frame->free(&m_shadersHandleAlloc);
        frame->free(&m_texturesHandleAlloc);
        frame->free(&m_vertexLayoutsHandleAlloc);
        frame->free(&m_vertexBuffersHandleAlloc);
        frame->free(&m_indexBuffersHandleAlloc);
    }

    FrameBufferHandle createFrameBuffer(FrameBufferSpecification specification) {
        FrameBufferHandle handle = m_frameBuffersHandleAlloc.alloc();
        CreateFrameBufferCommand cmd(handle, specification);
        m_submit->getPreCommandQueue().write(cmd);
        return handle;
    }

    uint32_t readFrameBuffer(
        FrameBufferHandle handle, int attachIndex, int x, int y, int width, int height, void *data
    ) {
        ReadFrameBufferCommand cmd(handle, attachIndex, x, y, width, height, data);
        m_submit->getPostCommandQueue().write(cmd);
        return m_frameNumber + 2;
    }

    void deleteFrameBuffer(FrameBufferHandle handle) {
        m_submit->queueFree(handle);
        DeleteFrameBufferCommand cmd(handle);
        m_submit->getPostCommandQueue().write(cmd);
    }

    ProgramHandle createProgram(ProgramCreate create) {
        ProgramHandle handle = m_shadersHandleAlloc.alloc();
        CreateProgramCommand cmd(handle, create);
        m_submit->getPreCommandQueue().write(cmd);
        return handle;
    }

    ProgramCreate compileProgram(ProgramCompile compile) {
        return m_renderer->compileProgram(compile);
    }

    void deleteProgram(ProgramHandle handle) {
        m_submit->queueFree(handle);
        DeleteProgramCommand cmd(handle);
        m_submit->getPostCommandQueue().write(cmd);
    }

    TextureHandle createTexture(TextureCreate create) {
        TextureHandle handle = m_texturesHandleAlloc.alloc();
        CreateTextureCommand cmd(handle, create);
        m_submit->getPreCommandQueue().write(cmd);
        return handle;
    }

    void updateTexture(TextureHandle handle, Foundation::Memory mem) {
        UpdateTextureCommand cmd(handle, mem);
        m_submit->getPreCommandQueue().write(cmd);
    }

    void resizeTexture(TextureHandle handle, uint32_t width, uint32_t height) {
        ResizeTextureCommand cmd(handle, width, height);
        m_submit->getPreCommandQueue().write(cmd);
    }

    void deleteTexture(TextureHandle handle) {
        m_submit->queueFree(handle);
        DeleteTextureCommand cmd(handle);
        m_submit->getPostCommandQueue().write(cmd);
    }

    IndexBufferHandle
    createIndexBuffer(Foundation::Memory indices, BufferElementType elementType, size_t count) {
        IndexBufferHandle handle = m_indexBuffersHandleAlloc.alloc();
        CreateIndexBufferCommand cmd(handle, indices, elementType, count);
        m_submit->getPreCommandQueue().write(cmd);
        return handle;
    }

    IndexBufferHandle createDynamicIndexBuffer(
        Foundation::Memory indices, BufferElementType elementType, size_t count
    ) {
        IndexBufferHandle handle = m_indexBuffersHandleAlloc.alloc();
        CreateDynamicIndexBufferCommand cmd(handle, indices, elementType, count);
        m_submit->getPreCommandQueue().write(cmd);
        return handle;
    }

    void
    updateDynamicIndexBuffer(IndexBufferHandle handle, Foundation::Memory indices, size_t count) {
        UpdateDynamicIndexBufferCommand cmd(handle, indices, count);
        m_submit->getPreCommandQueue().write(cmd);
    }

    void deleteIndexBuffer(IndexBufferHandle handle) {
        m_submit->queueFree(handle);
        DeleteIndexBufferCommand cmd(handle);
        m_submit->getPostCommandQueue().write(cmd);
    }

    VertexBufferHandle
    createVertexBuffer(Foundation::Memory data, uint32_t size, VertexLayoutHandle layoutHandle) {
        VertexBufferHandle handle = m_vertexBuffersHandleAlloc.alloc();
        CreateVertexBufferCommand cmd(handle, data, size, layoutHandle);
        m_submit->getPreCommandQueue().write(cmd);
        return handle;
    }

    VertexBufferHandle createDynamicVertexBuffer(
        Foundation::Memory data,
        uint32_t size,
        VertexLayoutHandle layoutHandle = MIREN_INVALID_HANDLE
    ) {
        VertexBufferHandle handle = m_vertexBuffersHandleAlloc.alloc();
        CreateDynamicVertexBufferCommand cmd(handle, data, size, layoutHandle);
        m_submit->getPreCommandQueue().write(cmd);
        return handle;
    }

    void
    updateDynamicVertexBuffer(VertexBufferHandle handle, Foundation::Memory data, uint32_t size) {
        UpdateDynamicVertexBufferCommand cmd(handle, data, size);
        m_submit->getPreCommandQueue().write(cmd);
    }

    void deleteVertexBuffer(VertexBufferHandle handle) {
        m_submit->queueFree(handle);
        DeleteVertexBufferCommand cmd(handle);
        m_submit->getPostCommandQueue().write(cmd);
    }

    VertexLayoutHandle createVertexLayout(VertexBufferLayoutData data) {
        VertexLayoutHandle handle = m_vertexLayoutsHandleAlloc.alloc();
        CreateVertexLayoutCommand cmd(handle, data);
        m_submit->getPreCommandQueue().write(cmd);
        return handle;
    }

    void deleteVertexLayout(VertexLayoutHandle handle) {
        m_submit->queueFree(handle);
        DeleteVertexLayoutCommand cmd(handle);
        m_submit->getPostCommandQueue().write(cmd);
    }

    void destroyTransientVertexBuffer(TransientVertexBuffer &tvb) {
        deleteVertexBuffer(tvb.handle);
        F_FREE(Foundation::getAllocator(), tvb.data);
    }

    void destroyTransientIndexBuffer(TransientIndexBuffer &tib) {
        deleteIndexBuffer(tib.handle);
        F_FREE(Foundation::getAllocator(), tib.data);
    }

    TransientIndexBuffer createTransientIndexBuffer(uint32_t size) {
        TransientIndexBuffer tib;
        tib.data = (uint8_t *)F_ALLOC(Foundation::getAllocator(), size);
        tib.size = size;
        tib.startIndex = 0;
        tib.handle = createDynamicIndexBuffer(nullptr, BufferElementType::UnsignedShort, size / 2);
        tib.elementType = BufferElementType::UnsignedShort;
        return tib;
    }

    TransientVertexBuffer createTransientVertexBuffer(uint32_t size) {
        TransientVertexBuffer tvb;
        tvb.data = (uint8_t *)F_ALLOC(Foundation::getAllocator(), size);
        tvb.size = size;
        tvb.startVertex = 0;
        tvb.handle = createDynamicVertexBuffer(nullptr, size);
        return tvb;
    }

    void allocTransientVertexBuffer(TransientVertexBuffer *buffer, uint32_t size) {
        uint32_t transientVBOffset = m_submit->m_transientVbSize;
        buffer->data = &m_submit->m_transientVb.data[transientVBOffset];
        m_submit->m_transientVbSize += size;
        PND_ASSERT(
            m_submit->m_transientVbSize < TRANSIENT_VERTEX_BUFFER_SIZE, "INCREASE DEFAULT VB SIZE"
        );
        buffer->size = size;
        buffer->startVertex = transientVBOffset;
        buffer->handle = m_submit->m_transientVb.handle;
    }

    void allocTransientIndexBuffer(
        TransientIndexBuffer *buffer, uint32_t count, BufferElementType elementType
    ) {
        uint32_t transientIBOffset = m_submit->m_transientIbSize;
        uint32_t elementSize = VertexBufferElement::getSizeOfType(elementType);
        uint32_t size = count * elementSize;
        buffer->data = &m_submit->m_transientIb.data[transientIBOffset];
        m_submit->m_transientIbSize += size;
        PND_ASSERT(
            m_submit->m_transientIbSize < TRANSIENT_INDEX_BUFFER_SIZE, "INCREASE DEFAULT IB SIZE"
        );
        buffer->size = size;
        buffer->startIndex = transientIBOffset;
        buffer->elementType = elementType;
        buffer->handle = m_submit->m_transientIb.handle;
    }

    uint32_t readTexture(TextureHandle handle, void *data) {
        ReadTextureCommand cmd(handle, data);
        m_submit->getPostCommandQueue().write(cmd);
        return m_frameNumber + 1;
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

    void setShader(ProgramHandle handle) {
        m_submit->setShader(handle);
    }

    void addInput(
        ProgramHandle handle,
        const char *name,
        RenderPassInputType type,
        const void *value,
        size_t size
    ) {
        m_submit->addInput(handle, name, type, value, size);
    }

    void setVertexLayout(VertexLayoutHandle handle) {
        m_submit->setVertexLayout(handle);
    }

    void discard() {
        m_submit->resetCurrentDrawCall();
    }

    void submit(ViewId id) {
        m_submit->submitCurrentDrawCall(id);
        m_submit->beginDrawCall();
    }

    void swap() {
        Foundation::swap(m_render, m_submit);
    }

    // Main thread
    uint32_t frame() {
        m_rendererSemaphore.wait();
        freeAllHandles(m_submit);
        for (int i = 0; i < MAX_VIEWS; i++) {
            m_submit->m_views[i] = m_views[i];
        }
        swap();
        m_submit->reset();
        m_apiSemaphore.post();
        return m_frameNumber++;
    }

    uint32_t getFrameNumber() {
        return m_frameNumber;
    }

    void setViewport(ViewId id, Rect rect) {
        m_views[id].m_viewport = rect;
    }

    void setViewClear(ViewId id, uint32_t color) {
        m_views[id].m_clearColor = color;
    }

    void setViewClearAttachments(ViewId id, std::vector<Clear> clear) {
        m_views[id].m_clearAttachments = clear;
    }

    void setViewFrameBuffer(ViewId id, FrameBufferHandle frameBuffer) {
        m_views[id].m_frameBuffer = frameBuffer;
    }

    Foundation::Thread m_thread;

private:
    RendererI *m_renderer;
    Frame m_frame[2];
    Frame *m_render;
    Frame *m_submit;
    View m_views[MAX_VIEWS];
    uint32_t m_frameNumber;

    // TODO: - Add reusable buffers:
    // DynamicIndexBuffer m_dynamicIndexBuffers[MAX_DYNAMIC_INDEX_BUFFERS];
    // DynamicVertexBuffer m_dynamicVertexBuffers[MAX_DYNAMIC_VERTEX_BUFFERS];

    Fern::GraphicsContext *m_ctx;
    HandleAllocator<FrameBufferHandle> m_frameBuffersHandleAlloc;
    HandleAllocator<ProgramHandle> m_shadersHandleAlloc;
    HandleAllocator<TextureHandle> m_texturesHandleAlloc;
    HandleAllocator<VertexLayoutHandle> m_vertexLayoutsHandleAlloc;
    HandleAllocator<VertexBufferHandle> m_vertexBuffersHandleAlloc;
    HandleAllocator<IndexBufferHandle> m_indexBuffersHandleAlloc;

public:
    Foundation::Semaphore m_rendererSemaphore;
    Foundation::Semaphore m_apiSemaphore;
};

} // namespace Miren
