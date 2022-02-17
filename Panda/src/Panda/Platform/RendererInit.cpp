#include "pndpch.hpp"
#include "RendererInit.hpp"

#include "Panda/Renderer/Implementation/OpenGL/OpenGLRenderer.hpp"
#include "Panda/Renderer/Implementation/OpenGL/OpenGLShader.hpp"
#include "Panda/Renderer/Implementation/OpenGL/OpenGLTexture.hpp"
#include "Panda/Renderer/Implementation/OpenGL/OpenGLIndexBuffer.hpp"
#include "Panda/Renderer/Implementation/OpenGL/OpenGLVertexBuffer.hpp"
#include "Panda/Renderer/Implementation/OpenGL/OpenGLVertexBufferLayout.hpp"

namespace Panda {

IRenderer *RendererInit::getRenderer() {
    switch (currentRendererApi) {
        case RendererApi::None:
            PND_CRITICAL("RENDERER API NOT SPECIFIED");
        case RendererApi::OpenGL:
            return new OpenGLRenderer();
    }
}

IShader *RendererInit::createShader(const char *vertexPath, const char *fragmentPath) {
    switch (currentRendererApi) {
        case RendererApi::None:
            PND_CRITICAL("RENDERER API NOT SPECIFIED");
        case RendererApi::OpenGL:
            return new OpenGLShader(vertexPath, fragmentPath);
    }
}

ITexture *RendererInit::createTexture(const char *path) {
    switch (currentRendererApi) {
        case RendererApi::None:
            PND_CRITICAL("RENDERER API NOT SPECIFIED");
        case RendererApi::OpenGL:
            return new OpenGLTexture(path);
    }
}
IndexBuffer *RendererInit::createIndexBuffer(unsigned int *indices, unsigned int count, bool isDynamic) {
    switch (currentRendererApi) {
        case RendererApi::None:
            PND_CRITICAL("RENDERER API NOT SPECIFIED");
        case RendererApi::OpenGL:
            return new OpenGLIndexBuffer(indices, count, isDynamic);
    }
}

IVertexBuffer *RendererInit::createVertexBuffer(Vertex *vertices, unsigned int count, bool isDynamic) {
    switch (currentRendererApi) {
        case RendererApi::None:
            PND_CRITICAL("RENDERER API NOT SPECIFIED");
        case RendererApi::OpenGL:
            return new OpenGLVertexBuffer(vertices, count, isDynamic);
    };
}

IVertexBuffer *RendererInit::createVertexBuffer(float *data, unsigned int count, bool isDynamic, VertexBufferLayout *layout) {
    switch (currentRendererApi) {
        case RendererApi::None:
            PND_CRITICAL("RENDERER API NOT SPECIFIED");
        case RendererApi::OpenGL:
            return new OpenGLVertexBuffer(data, count, isDynamic, layout);
    };
}

VertexBufferLayout *RendererInit::createVertexBufferLayout() {
    switch (currentRendererApi) {
        case RendererApi::None:
            PND_CRITICAL("RENDERER API NOT SPECIFIED");
        case RendererApi::OpenGL:
            return new OpenGLVertexBufferLayout();
    }
}

} // namespace Panda
