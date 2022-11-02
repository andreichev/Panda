#pragma once

#include "Miren/Miren.hpp"
#include "MirenHandleAllocator.hpp"
#include "CommandBuffer/Frame.hpp"
#include "CommandBuffer/RenderDraw.hpp"
#include "CommandBuffer/CommandQueue.hpp"
#include "MirenStates.hpp"
#include "RendererI.hpp"
#include "Platform/RendererImpl/OpenGL/RendererOpenGL.hpp"

#include <Foundation/Thread.hpp>
#include <Foundation/Semaphore.hpp>

namespace Miren {

uint32_t maxHandles = 1000;

void rendererExecuteCommands();
TransientVertexBuffer *createTransientVertexBuffer(uint32_t _size, VertexBufferLayoutData layout);
TransientIndexBuffer *createTransientIndexBuffer(uint32_t _size);

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
        m_thread.shutdown();
    }

    static int32_t renderThread(Foundation::Thread *_thread, void *_userData) {
        while (true) {
            Miren::renderFrame();
        }
        Miren::terminate();
        return 0;
    }

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
