//
// Created by Admin on 10.03.2022.
//

#include "Miren/Miren_p.hpp"

namespace Miren {

static Context *s_context = nullptr;
static const int CONTEXT_ALIGNMENT = 64;

void initialize() {
    s_context = ALIGNED_NEW(Foundation::getAllocator(), Context, CONTEXT_ALIGNMENT);
    s_context->init();
}

void terminate() {
    s_context->shutdown();
    ALIGNED_DELETE(Foundation::getAllocator(), s_context, CONTEXT_ALIGNMENT);
}

ShaderHandle createShader(const char *vertexPath, const char *fragmentPath) {
    return s_context->createShader(vertexPath, fragmentPath);
}

void deleteShader(ShaderHandle handle) {
    s_context->deleteShader(handle);
}

TextureHandle createTextureFromFile(const char *path) {
    return s_context->createTextureFromFile(path);
}

TextureHandle createTextureFromPixels(void *pixels, int width, int height) {
    return s_context->createTextureFromPixels(pixels, width, height);
}

void deleteTexture(TextureHandle handle) {
    s_context->deleteTexture(handle);
}

IndexBufferHandle createIndexBuffer(void *indices, BufferElementType elementType, size_t count) {
    return s_context->createIndexBuffer(indices, elementType, count);
}

IndexBufferHandle createDynamicIndexBuffer(void *indices, BufferElementType elementType, size_t count) {
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

VertexBufferHandle createDynamicVertexBuffer(void *data, uint32_t size, VertexLayoutHandle layoutHandle) {
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

void allocTransientVertexBuffer(TransientVertexBuffer *buffer, int32_t count, const VertexLayoutHandle layoutHandle) {}

void renderFrame() {
    s_context->renderFrame();
}

// TODO: - Implement, add swapping frames
void frame() {}

void setState(uint32_t state) {
    s_context->setState(state);
}

void setScissorRect(Rect rect) {
    s_context->setScissorRect(rect);
}

void setVertexBuffer(VertexBufferHandle handle) {
    s_context->setVertexBuffer(handle);
}

void setIndexBuffer(IndexBufferHandle handle, void *offset, size_t count) {
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

void submit() {
    s_context->submit();
}

void submitPrimitives(uint32_t elements) {
    s_context->submitPrimitives(elements);
}

void renderSemaphoreWait() {
    s_context->m_rendererSemaphore.wait();
}

void renderSemaphorePost() {
    s_context->m_rendererSemaphore.post();
}

} // namespace Miren