//
// Created by Admin on 14.03.2022.
//

#include "Frame.hpp"

namespace Panda {

Frame::Frame()
    : m_drawCalls() {
    begin();
}

void Frame::begin() {
    RenderDraw *draw = new RenderDraw();
    m_drawCalls.push(draw);
}

RenderDraw *Frame::popDrawCall() {
    if (m_drawCalls.empty()) {
        return nullptr;
    }
    RenderDraw *draw = m_drawCalls.front();
    m_drawCalls.pop();
    return draw;
}

void Frame::free(const RenderDraw *draw) {
    delete draw;
}

void Frame::setIndexBuffer(IndexBufferHandle handle, uint32_t count) {
    RenderDraw *draw = m_drawCalls.back();
    draw->m_indexBuffer = handle;
    draw->m_numIndices = count;
}

void Frame::setVertexBuffer(VertexBufferHandle handle) {
    RenderDraw *draw = m_drawCalls.back();
    draw->m_vertexBuffer = handle;
}

void Frame::setShader(ShaderHandle handle) {
    RenderDraw *draw = m_drawCalls.back();
    draw->m_shader = handle;
}

void Frame::setUniform(ShaderHandle handle, const char *name, void *value, uint16_t size) {
    RenderDraw *draw = m_drawCalls.back();
    draw->m_uniformBuffer.emplace(handle, name, value, size);
}

void Frame::setTexture(TextureHandle textureHandle, uint32_t slot) {
    RenderDraw *draw = m_drawCalls.back();
    draw->m_textureBindings.emplace(textureHandle, slot);
}

void Frame::submitCurrentDrawCall() {
    RenderDraw *draw = m_drawCalls.back();
    draw->isSubmitted = true;
}

} // namespace Panda
