//
// Created by Admin on 13.03.2022.
//

#pragma once

#include "Panda/Renderer/RendererI.hpp"

namespace Panda {

struct Uniform {
    ShaderHandle handle;
    const char *name;
    void *value;
    uint32_t size;

    Uniform(ShaderHandle handle, const char *name, void *value, uint32_t size)
        : handle(handle)
        , name(name)
        , value(value)
        , size(size) {}
};

struct TextureBinding {
    TextureHandle m_handle;
    uint32_t m_slot;
    TextureBinding(TextureHandle mHandle, uint32_t mSlot)
        : m_handle(mHandle)
        , m_slot(mSlot) {}
};

struct RenderDraw {
    RenderDraw()
        : isSubmitted(false)
        , m_numIndices(0)
        , m_shader(0)
        , m_indexBuffer(0)
        , m_vertexBuffer(0)
        , m_uniformBuffer()
        , m_textureBindings() {}

    bool isSubmitted;
    uint32_t m_numIndices;
    ShaderHandle m_shader;
    IndexBufferHandle m_indexBuffer;
    VertexBufferHandle m_vertexBuffer;
    std::queue<Uniform> m_uniformBuffer;
    std::queue<TextureBinding> m_textureBindings;
};

} // namespace Panda
