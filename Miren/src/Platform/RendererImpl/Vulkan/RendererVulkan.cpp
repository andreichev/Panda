//
// Created by Admin on 14.03.2022.
//

#include "RendererVulkan.hpp"

#include <Foundation/Assert.hpp>
#include <Foundation/Allocator.hpp>
#include <Foundation/Logger.hpp>

namespace Miren {

RendererVulkan::RendererVulkan(Fern::GraphicsContext *ctx)
    : m_uselessVao(0)
    , m_frameBuffers()
    , m_shaders()
    , m_indexBuffers()
    , m_vertexLayouts()
    , m_vertexBuffers()
    , m_textures()
    , m_ctx(ctx) {

}

RendererVulkan::~RendererVulkan() {
}

RendererType RendererVulkan::getRendererType() const {

}

void RendererVulkan::createFrameBuffer(
    FrameBufferHandle handle, FrameBufferSpecification specification
) {
}

void RendererVulkan::readFrameBuffer(
    FrameBufferHandle handle, int attachIndex, int x, int y, int width, int height, void *data
) {
}

void RendererVulkan::deleteFrameBuffer(FrameBufferHandle handle) {
}

void RendererVulkan::createProgram(ProgramHandle handle, ProgramCreate create) {
}

void RendererVulkan::deleteShader(ProgramHandle handle) {
}

void RendererVulkan::createTexture(TextureHandle handle, TextureCreate create) {
}

void RendererVulkan::resizeTexture(TextureHandle handle, uint32_t width, uint32_t height) {
}

void RendererVulkan::deleteTexture(TextureHandle handle) {
}

void RendererVulkan::createIndexBuffer(
    IndexBufferHandle handle,
    Foundation::Memory indices,
    BufferElementType elementType,
    size_t count
) {
    m_indexBuffers[handle.id].create(indices.data, elementType, count, false);
    indices.release();
}

void RendererVulkan::createDynamicIndexBuffer(
    IndexBufferHandle handle,
    Foundation::Memory indices,
    BufferElementType elementType,
    size_t count
) {
}

void RendererVulkan::updateDynamicIndexBuffer(
    IndexBufferHandle handle, Foundation::Memory indices, size_t count
) {
}

void RendererVulkan::deleteIndexBuffer(IndexBufferHandle handle) {
}

void RendererVulkan::createVertexBuffer(
    VertexBufferHandle handle,
    Foundation::Memory data,
    uint32_t size,
    VertexLayoutHandle layoutHandle
) {
}

void RendererVulkan::createDynamicVertexBuffer(
    VertexBufferHandle handle,
    Foundation::Memory data,
    uint32_t size,
    VertexLayoutHandle layoutHandle
) {
}

void RendererVulkan::updateDynamicVertexBuffer(
    VertexBufferHandle handle, Foundation::Memory data, uint32_t size
) {
}

void RendererVulkan::deleteVertexBuffer(VertexBufferHandle handle) {
}

void RendererVulkan::createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) {
}

void RendererVulkan::deleteVertexLayout(VertexLayoutHandle handle) {}

void RendererVulkan::readTexture(Miren::TextureHandle handle, void *data) {
}

void RendererVulkan::setUniform(const Uniform &uniform) {
    switch (uniform.type) {
        case UniformType::Sampler:
            return;
        case UniformType::Mat3:
            return;
        case UniformType::Mat4:
            return;
        case UniformType::Vec4:
            return;
        case UniformType::Count:
            return;
    }
    LOG_ERROR("UNIFORM TYPE IS UNDEFINED");
}

void RendererVulkan::setTexture(TextureHandle handle, uint32_t slot) {
}

void RendererVulkan::submit(Frame *frame) {

}

void RendererVulkan::viewChanged(View &view) {

}

void RendererVulkan::submit(RenderDraw *draw) {
    // TODO: Capture time

}

} // namespace Miren
