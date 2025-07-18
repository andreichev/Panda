//
// Created by Admin on 10.03.2022.
//

#include "Miren/Miren_p.hpp"

namespace Miren {

// MARK: - PUBLIC METHODS IMPL

static Context *s_context = nullptr;

void initialize(Fern::GraphicsContext *ctx) {
    MIREN_LOG("MIREN INIT BEGIN");
    MIREN_LOG("ALLOCATING MIREN CONTEXT, {} BYTES", sizeof(Context));
    MIREN_LOG("FRAME DATA SIZE: {} BYTES", sizeof(Frame));
    s_context = F_NEW(Foundation::getAllocator(), Context);
    s_context->init(ctx);
    MIREN_LOG("MIREN INIT END");
}

void terminate() {
    MIREN_LOG("MIREN SHUTDOWN BEGIN");
    s_context->shutdown();
    F_DELETE(Foundation::getAllocator(), s_context);
    s_context = nullptr;
    MIREN_LOG("MIREN SHUTDOWN END");
}

FrameBufferHandle createFrameBuffer(FrameBufferSpecification specification) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    return s_context->createFrameBuffer(specification);
}

uint32_t readFrameBuffer(
    FrameBufferHandle handle, int attachIndex, int x, int y, int width, int height, void *data
) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    return s_context->readFrameBuffer(handle, attachIndex, x, y, width, height, data);
}

void deleteFrameBuffer(FrameBufferHandle handle) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->deleteFrameBuffer(handle);
}

ProgramCreate compileProgram(ProgramCompile compile) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    return s_context->compileProgram(compile);
}

ProgramHandle createProgram(ProgramCreate create) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    return s_context->createProgram(create);
}

void deleteProgram(ProgramHandle handle) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->deleteProgram(handle);
}

TextureHandle createTexture(TextureCreate create) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    PND_ASSERT_F(
        create.m_width > 0 && create.m_height > 0,
        "INVALID TEXTURE SIZE w: %d, h: %d",
        create.m_width,
        create.m_height
    );
    return s_context->createTexture(create);
}

void updateTexture(TextureHandle handle, Foundation::Memory mem) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->updateTexture(handle, mem);
}

void resizeTexture(TextureHandle handle, uint32_t width, uint32_t height) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    PND_ASSERT_F(width > 0 && height > 0, "INVALID TEXTURE SIZE w: %d, h: %d", width, height);
    s_context->resizeTexture(handle, width, height);
}

void deleteTexture(TextureHandle handle) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->deleteTexture(handle);
}

IndexBufferHandle
createIndexBuffer(Foundation::Memory indices, BufferElementType elementType, size_t count) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    return s_context->createIndexBuffer(indices, elementType, count);
}

IndexBufferHandle
createDynamicIndexBuffer(Foundation::Memory indices, BufferElementType elementType, size_t count) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    return s_context->createDynamicIndexBuffer(indices, elementType, count);
}

void updateDynamicIndexBuffer(IndexBufferHandle handle, Foundation::Memory indices, size_t count) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->updateDynamicIndexBuffer(handle, indices, count);
}

void deleteIndexBuffer(IndexBufferHandle handle) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->deleteIndexBuffer(handle);
}

VertexBufferHandle
createVertexBuffer(Foundation::Memory data, uint32_t size, VertexLayoutHandle layoutHandle) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    return s_context->createVertexBuffer(data, size, layoutHandle);
}

VertexBufferHandle
createDynamicVertexBuffer(Foundation::Memory data, uint32_t size, VertexLayoutHandle layoutHandle) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    return s_context->createDynamicVertexBuffer(data, size, layoutHandle);
}

void updateDynamicVertexBuffer(VertexBufferHandle handle, Foundation::Memory data, uint32_t size) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->updateDynamicVertexBuffer(handle, data, size);
}

void deleteVertexBuffer(VertexBufferHandle handle) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->deleteVertexBuffer(handle);
}

VertexLayoutHandle createVertexLayout(VertexBufferLayoutData data) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    return s_context->createVertexLayout(data);
}

void deleteVertexLayout(VertexLayoutHandle handle) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->deleteVertexLayout(handle);
}

uint32_t readTexture(TextureHandle handle, void *data) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    return s_context->readTexture(handle, data);
}

void allocTransientVertexBuffer(TransientVertexBuffer *buffer, uint32_t size) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->allocTransientVertexBuffer(buffer, size);
}

void allocTransientIndexBuffer(
    TransientIndexBuffer *buffer, uint32_t count, BufferElementType elementType
) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->allocTransientIndexBuffer(buffer, count, elementType);
}

bool renderFrame() {
    // PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    if (!s_context) { return false; }
    return s_context->renderFrame();
}

uint32_t frame() {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    return s_context->frame();
}

uint32_t getFrameNumber() {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    return s_context->getFrameNumber();
}

void setViewClear(ViewId id, uint32_t color) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->setViewClear(id, color);
}

void setViewClearAttachments(ViewId id, std::vector<Clear> clear) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->setViewClearAttachments(id, clear);
}

void setViewport(ViewId id, Rect rect) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->setViewport(id, rect);
}

void setViewFrameBuffer(ViewId id, FrameBufferHandle frameBuffer) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->setViewFrameBuffer(id, frameBuffer);
}

void setState(uint32_t state) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->setState(state);
}

void setScissorRect(Rect rect) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->setScissorRect(rect);
}

void setVertexBuffer(VertexBufferHandle handle, intptr_t offset) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->setVertexBuffer(handle, offset);
}

void setIndexBuffer(IndexBufferHandle handle, intptr_t offset, size_t count) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->setIndexBuffer(handle, offset, count);
}

void setShader(ProgramHandle handle) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->setShader(handle);
}

void setUniform(ProgramHandle handle, const char *name, void *value, UniformType type, int count) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->setUniform(handle, name, value, type, count);
}

void setTexture(TextureHandle textureHandle, uint32_t slot) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->setTexture(textureHandle, slot);
}

void setVertexLayout(VertexLayoutHandle handle) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->setVertexLayout(handle);
}

void discard() {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->discard();
}

void submit(ViewId id) {
    PND_ASSERT(s_context != nullptr, "MIREN NOT INITIALIZED");
    s_context->submit(id);
}

} // namespace Miren
