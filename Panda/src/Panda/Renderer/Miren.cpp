//
// Created by Admin on 10.03.2022.
//

#include "pndpch.hpp"
#include "Miren.hpp"

#include "Platform/RendererImpl/OpenGL/RendererOpenGL.hpp"

namespace Panda {

RendererI *Miren::s_context = nullptr;
Frame Miren::s_frame = Frame();
int Miren::s_shadersCount = 0;
int Miren::s_texturesCount = 0;
int Miren::s_vertexLayoutsCount = 0;
int Miren::s_vertexBuffersCount = 0;
int Miren::s_indexBuffersCount = 0;
CommandQueue Miren::s_commandQueue;

void Miren::initialize(GSize size) {
    s_context = new RendererOpenGL(size);
    // TODO: - delete s_context somewhere
}

VertexBufferHandle Miren::createVertexBuffer(Vertex *vertices, unsigned int count, bool isDynamic) {
    VertexBufferLayoutData layoutData;
    layoutData.pushVector();
    VertexLayoutHandle layoutHandle = createVertexLayout(layoutData);
    return Miren::createVertexBuffer((float *)vertices, count * 6, isDynamic, layoutHandle);
}

VertexBufferHandle Miren::createVertexBuffer(float *data, unsigned int count, bool isDynamic, VertexLayoutHandle layoutHandle) {
    VertexBufferHandle handle = s_vertexBuffersCount;
    s_vertexBuffersCount++;
    s_commandQueue.post(new CreateVertexBufferCommand(handle, data, count, isDynamic, layoutHandle));
    return handle;
}

VertexLayoutHandle Miren::createVertexLayout(VertexBufferLayoutData data) {
    VertexLayoutHandle handle = s_vertexLayoutsCount;
    s_vertexLayoutsCount++;
    s_commandQueue.post(new CreateVertexLayoutCommand(handle, data));
    return handle;
}

ShaderHandle Miren::createShader(const char *vertexPath, const char *fragmentPath) {
    ShaderHandle handle = s_shadersCount;
    s_shadersCount++;
    s_commandQueue.post(new CreateShaderCommand(handle, vertexPath, fragmentPath));
    return handle;
}

TextureHandle Miren::createTexture(const char *path) {
    TextureHandle handle = s_texturesCount;
    s_texturesCount++;
    s_commandQueue.post(new CreateTextureCommand(handle, path));
    return handle;
}

IndexBufferHandle Miren::createIndexBuffer(unsigned int *indices, unsigned int count, bool isDynamic) {
    IndexBufferHandle handle = s_indexBuffersCount;
    s_indexBuffersCount++;
    s_commandQueue.post(new CreateIndexBufferCommand(handle, indices, count, isDynamic));
    return handle;
}

void Miren::beginFrameProcessing() {
    s_context->semaphoreWait();
}

void Miren::endFrameProcessing() {
    s_context->semaphoreSignal();
}

void Miren::renderFrame() {
    if (s_context == nullptr) {
        return;
    }
    s_context->semaphoreWait();
    s_context->clear();
    rendererExecuteCommands();
    RenderDraw *draw;
    while ((draw = s_frame.popDrawCall()) != nullptr) {
        if (draw->isSubmitted == false || draw->m_numIndices == 0) {
            s_frame.free(draw);
            continue;
        }
        while (draw->m_textureBindings.empty() == false) {
            TextureBinding& textureBinding = draw->m_textureBindings.front();
            s_context->setTexture(textureBinding.m_handle, textureBinding.m_slot);
            draw->m_textureBindings.pop();
        }
        while (draw->m_uniformBuffer.empty() == false) {
            Uniform& uniform = draw->m_uniformBuffer.front();
            s_context->setUniform(uniform.handle, uniform.name, uniform.value, uniform.size);
            draw->m_uniformBuffer.pop();
        }
        s_context->submit(draw->m_shader, draw->m_vertexBuffer, draw->m_indexBuffer, draw->m_numIndices);
        s_frame.free(draw);
    }
    s_context->flip();
    s_frame.beginDrawCall();
    s_context->semaphoreSignal();
}

void Miren::rendererExecuteCommands() {
    const RendererCommand *command;
    while ((command = s_commandQueue.poll()) != nullptr) {
        switch (command->type) {
            case RendererCommandType::CreateVertexLayout: {
                const CreateVertexLayoutCommand *cmd = static_cast<const CreateVertexLayoutCommand *>(command);
                s_context->createVertexLayout(cmd->handle, cmd->data);
                break;
            }
            case RendererCommandType::CreateVertexBuffer: {
                const CreateVertexBufferCommand *cmd = static_cast<const CreateVertexBufferCommand *>(command);
                s_context->createVertexBuffer(cmd->handle, cmd->data, cmd->count, cmd->isDynamic, cmd->layoutHandle);
                break;
            }
            case RendererCommandType::CreateIndexBuffer: {
                const CreateIndexBufferCommand *cmd = static_cast<const CreateIndexBufferCommand *>(command);
                s_context->createIndexBuffer(cmd->handle, cmd->indices, cmd->count, cmd->isDynamic);
                break;
            }
            case RendererCommandType::CreateShader: {
                const CreateShaderCommand *cmd = static_cast<const CreateShaderCommand *>(command);
                s_context->createShader(cmd->handle, cmd->vertexPath, cmd->fragmentPath);
                break;
            }
            case RendererCommandType::CreateTexture: {
                const CreateTextureCommand *cmd = static_cast<const CreateTextureCommand *>(command);
                s_context->createTexture(cmd->handle, cmd->path);
                break;
            }
        }
        s_commandQueue.release(command);
    }
}

void Miren::setVertexBuffer(VertexBufferHandle handle) {
    s_frame.setVertexBuffer(handle);
}

void Miren::setIndexBuffer(IndexBufferHandle handle, uint32_t count) {
    s_frame.setIndexBuffer(handle, count);
}

void Miren::setShader(ShaderHandle handle) {
    s_frame.setShader(handle);
}

void Miren::setUniform(ShaderHandle handle, const char *name, void *value, uint16_t size) {
    s_frame.setUniform(handle, name, value, size);
}

void Miren::setTexture(TextureHandle textureHandle, uint32_t slot) {
    s_frame.setTexture(textureHandle, slot);
}

void Miren::submit(ShaderHandle shader, VertexBufferHandle vertexBuffer, IndexBufferHandle indexBuffer, uint32_t indicesCount) {
    s_frame.setShader(shader);
    s_frame.setVertexBuffer(vertexBuffer);
    s_frame.setIndexBuffer(indexBuffer, indicesCount);
    s_frame.submitCurrentDrawCall();
    s_frame.beginDrawCall();
}

void Miren::submit() {
    s_frame.submitCurrentDrawCall();
    s_frame.beginDrawCall();
}

} // namespace Panda
