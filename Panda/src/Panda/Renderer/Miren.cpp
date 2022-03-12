//
// Created by Admin on 10.03.2022.
//

#include "Miren.hpp"

namespace Panda {

int Miren::s_shadersCount = 0;
int Miren::s_texturesCount = 0;
int Miren::s_vertexLayoutsCount = 0;
int Miren::s_vertexBuffersCount = 0;
int Miren::s_indexBuffersCount = 0;
CommandQueue Miren::s_commandQueue;

Miren::Miren() {}

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
    rendererExecuteCommands();
}

void Miren::rendererExecuteCommands() {
    s_context->semaphoreWait();
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
    s_context->semaphoreSignal();
}

} // namespace Panda