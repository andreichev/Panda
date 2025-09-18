//
// Created by Admin on 13.03.2022.
//

#pragma once

#include "Miren/Base.hpp"
#include "Miren/MirenStates.hpp"
#include "RenderPassInputs.hpp"

namespace Miren {

struct RenderPass {
    RenderPass()
        : m_isSubmitted(false)
        , m_state(MIREN_STATE_CULL_FACE | MIREN_STATE_DEPTH_TEST)
        , m_numIndices(0)
        , m_textureBindingsCount(0)
        , m_indicesOffset(0)
        , m_verticesOffset(0)
        , m_viewId(0)
        , m_shader(MIREN_INVALID_HANDLE)
        , m_indexBuffer(MIREN_INVALID_HANDLE)
        , m_vertexBuffer(MIREN_INVALID_HANDLE)
        , m_vertexLayout(MIREN_INVALID_HANDLE)
        , m_scissorRect(Rect::zero())
        , m_inputs(1000) {}

    RenderPass(RenderPass &&other) = default;

    RenderPass &operator=(RenderPass &&other) = default;

    void reset() {
        m_isSubmitted = false;
        m_state = MIREN_STATE_CULL_FACE | MIREN_STATE_DEPTH_TEST;
        m_numIndices = 0;
        m_textureBindingsCount = 0;
        m_indicesOffset = 0;
        m_verticesOffset = 0;
        m_viewId = 0;
        m_shader = MIREN_INVALID_HANDLE;
        m_indexBuffer = MIREN_INVALID_HANDLE;
        m_vertexBuffer = MIREN_INVALID_HANDLE;
        m_vertexLayout = MIREN_INVALID_HANDLE;
        m_scissorRect = Rect::zero();
        m_inputs.reset();
    }

    void addInput(
        ProgramHandle handle,
        const char *name,
        RenderPassInputType type,
        const void *value,
        size_t size
    ) {
        PassInput input(handle, name, type, const_cast<void *>(value), size);
        m_inputs.writeInput(input);
    }

    bool m_isSubmitted;
    uint32_t m_numIndices;
    intptr_t m_indicesOffset;
    intptr_t m_verticesOffset;
    ProgramHandle m_shader;
    IndexBufferHandle m_indexBuffer;
    VertexBufferHandle m_vertexBuffer;
    VertexLayoutHandle m_vertexLayout;
    RenderPassInputs m_inputs;
    uint32_t m_textureBindingsCount;
    Rect m_scissorRect;
    ViewId m_viewId;
    uint32_t m_state;
};

} // namespace Miren
