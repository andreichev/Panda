//
// Created by Admin on 14.03.2022.
//

#include "Frame.hpp"

#include <Foundation/Allocator.hpp>

namespace Miren {

Frame::Frame()
    : m_drawCallsCount(0) {}

void Frame::beginDrawCall() {
    m_drawCallsCount++;
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.reset();
}

void Frame::setState(uint32_t state) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.m_state = state;
}

void Frame::setIndexBuffer(IndexBufferHandle handle, intptr_t offset, uint32_t count) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.m_indexBuffer = handle;
    draw.m_indicesOffset = offset;
    draw.m_numIndices = count;
}

void Frame::setVertexBuffer(VertexBufferHandle handle, intptr_t offset) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.m_vertexBuffer = handle;
    draw.m_verticesOffset = offset;
}

void Frame::setVertexLayout(VertexLayoutHandle handle) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.m_vertexLayout = handle;
}

void Frame::setShader(ShaderHandle handle) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.m_shader = handle;
}

void Frame::setFrameBuffer(FrameBufferHandle handle) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.m_frameBuffer = handle;
}

void Frame::setUniform(ShaderHandle handle, const char *name, void *value, UniformDataType type) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.addUniform(handle, name, value, type);
}

void Frame::setTexture(TextureHandle textureHandle, uint32_t slot) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.setTexture(textureHandle, slot);
}

void Frame::setScissorRect(Rect rect) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.m_scissorRect = rect;
}

void Frame::submitCurrentDrawCall(ViewId id) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.m_viewId = id;
    draw.m_isSubmitted = true;
}

uint32_t Frame::getDrawCallsCount() {
    return m_drawCallsCount;
}

RenderDraw *Frame::getDrawCalls() {
    return m_drawCalls;
}

void Frame::reset() {
    m_drawCallsCount = 0;
    m_transientVbSize = 0;
    m_transientIbSize = 0;
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.reset();
}

} // namespace Miren
