//
// Created by Admin on 10.03.2022.
//

#include "Miren/Miren_p.hpp"

namespace Miren {

static Context *s_context = nullptr;
static const int CONTEXT_ALIGNMENT = 64;

void initialize() {
    MIREN_LOG("MIREN INIT BEGIN");
    MIREN_LOG("ALLOCATING MIREN CONTEXT, {} BYTES", sizeof(Context));
    MIREN_LOG("FRAME DATA SIZE: {} BYTES", sizeof(Frame));
    s_context = ALIGNED_NEW(Foundation::getAllocator(), Context, CONTEXT_ALIGNMENT);
    MIREN_LOG("MIREN INIT END");
}

void terminate() {
    MIREN_LOG("MIREN SHUTDOWN BEGIN");
    ALIGNED_DELETE(Foundation::getAllocator(), s_context, CONTEXT_ALIGNMENT);
    s_context = nullptr;
    MIREN_LOG("MIREN SHUTDOWN END");
}

FrameBufferHandle createFrameBuffer(FrameBufferSpecification specification) {
    return s_context->createFrameBuffer(specification);
}

void resizeFrameBuffer(FrameBufferHandle handle, uint32_t width, uint32_t height) {
    s_context->resizeFrameBuffer(handle, width, height);
}

void deleteFrameBuffer(FrameBufferHandle handle) {
    s_context->deleteFrameBuffer(handle);
}

ShaderHandle createShader(const char *vertexCode, const char *fragmentCode) {
    return s_context->createShader(vertexCode, fragmentCode);
}

void deleteShader(ShaderHandle handle) {
    s_context->deleteShader(handle);
}

TextureHandle createTexture(TextureCreate create) {
    return s_context->createTexture(create);
}

void deleteTexture(TextureHandle handle) {
    s_context->deleteTexture(handle);
}

IndexBufferHandle createIndexBuffer(void *indices, BufferElementType elementType, size_t count) {
    return s_context->createIndexBuffer(indices, elementType, count);
}

IndexBufferHandle createDynamicIndexBuffer(
    void *indices, BufferElementType elementType, size_t count) {
    return s_context->createDynamicIndexBuffer(indices, elementType, count);
}

void updateDynamicIndexBuffer(IndexBufferHandle handle, void *indices, size_t count) {
    s_context->updateDynamicIndexBuffer(handle, indices, count);
}

void deleteIndexBuffer(IndexBufferHandle handle) {
    s_context->deleteIndexBuffer(handle);
}

VertexBufferHandle createVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
    return s_context->createVertexBuffer(data, size, layoutHandle);
}

VertexBufferHandle createDynamicVertexBuffer(
    void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
    return s_context->createDynamicVertexBuffer(data, size, layoutHandle);
}

void updateDynamicVertexBuffer(VertexBufferHandle handle, void *data, uint32_t size) {
    s_context->updateDynamicVertexBuffer(handle, data, size);
}

void deleteVertexBuffer(VertexBufferHandle handle) {
    s_context->deleteVertexBuffer(handle);
}

VertexLayoutHandle createVertexLayout(VertexBufferLayoutData data) {
    return s_context->createVertexLayout(data);
}

void deleteVertexLayout(VertexLayoutHandle handle) {
    s_context->deleteVertexLayout(handle);
}

void allocTransientVertexBuffer(TransientVertexBuffer *buffer, uint32_t size) {
    s_context->allocTransientVertexBuffer(buffer, size);
}

void allocTransientIndexBuffer(
    TransientIndexBuffer *buffer, uint32_t count, BufferElementType elementType) {
    s_context->allocTransientIndexBuffer(buffer, count, elementType);
}

bool renderFrame() {
    return s_context->renderFrame();
}

uint32_t frame() {
    return s_context->frame();
}

void setViewClear(ViewId id, uint32_t color) {
    s_context->setViewClear(id, color);
}

void setViewport(ViewId id, Rect rect) {
    s_context->setViewport(id, rect);
}

void setState(uint32_t state) {
    s_context->setState(state);
}

void setScissorRect(Rect rect) {
    s_context->setScissorRect(rect);
}

void setVertexBuffer(VertexBufferHandle handle, intptr_t offset) {
    s_context->setVertexBuffer(handle, offset);
}

void setIndexBuffer(IndexBufferHandle handle, intptr_t offset, size_t count) {
    s_context->setIndexBuffer(handle, offset, count);
}

void setShader(ShaderHandle handle) {
    s_context->setShader(handle);
}

void setUniform(ShaderHandle handle, const char *name, void *value, UniformDataType type) {
    s_context->setUniform(handle, name, value, type);
}

void setTexture(TextureHandle textureHandle, uint32_t slot) {
    s_context->setTexture(textureHandle, slot);
}

void setVertexLayout(VertexLayoutHandle handle) {
    s_context->setVertexLayout(handle);
}

void setFrameBuffer(FrameBufferHandle frameBuffer) {
    s_context->setFrameBuffer(frameBuffer);
}

void submit(ViewId id) {
    s_context->submit(id);
}

void renderSemaphoreWait() {
    s_context->m_rendererSemaphore.wait();
}

void renderSemaphorePost() {
    s_context->m_rendererSemaphore.post();
}

} // namespace Miren
