//
// Created by Admin on 13.03.2022.
//

#pragma once

#include "Miren/Base.hpp"
#include "Miren/MirenStates.hpp"
#include "Uniform.hpp"

#include <queue>

namespace Miren {

struct TextureBinding {
    TextureHandle m_handle;
    uint32_t m_slot;
    TextureBinding(TextureHandle mHandle, uint32_t mSlot)
        : m_handle(mHandle)
        , m_slot(mSlot) {}
};

// Вызов отрисовки кадра
struct RenderDraw {
    RenderDraw()
        : m_isSubmitted(false)
        , m_state(MIREN_STATE_CULL_FACE | MIREN_STATE_DEPTH_TEST)
        , m_isIndexed(true)
        , m_numIndices(0)
        , m_numElemets(0)
        , m_indicesOffset(nullptr)
        , m_shader(0)
        , m_indexBuffer(0)
        , m_vertexBuffer(0)
        , m_uniformBuffer()
        , m_textureBindings()
        , m_scissorRect(Rect::zero()) {}

    bool m_isSubmitted;
    // Отрисовка по индексам или примитив
    bool m_isIndexed;
    uint32_t m_numIndices;
    void *m_indicesOffset;
    // Если примитивы, а не индексы - их количество
    uint32_t m_numElemets;
    ShaderHandle m_shader;
    IndexBufferHandle m_indexBuffer;
    VertexBufferHandle m_vertexBuffer;
    std::queue<Uniform> m_uniformBuffer;
    std::queue<TextureBinding> m_textureBindings;
    Rect m_scissorRect;
    uint32_t m_state;
};

} // namespace Miren
