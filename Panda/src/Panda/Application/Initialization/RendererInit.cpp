#include "pndpch.hpp"
#include "RendererInit.hpp"

#include "Platform/RendererImpl/Context/OpenGLContext.hpp"
#include "Platform/RendererImpl/Context/GlesContext.hpp"
#include "Platform/RendererImpl/OpenGL/OpenGLRenderer.hpp"
#include "Platform/RendererImpl/OpenGL/OpenGLShader.hpp"
#include "Platform/RendererImpl/OpenGL/OpenGLTexture.hpp"
#include "Platform/RendererImpl/OpenGL/OpenGLIndexBuffer.hpp"
#include "Platform/RendererImpl/OpenGL/OpenGLVertexBuffer.hpp"
#include "Platform/RendererImpl/OpenGL/OpenGLVertexBufferLayout.hpp"

namespace Panda {

GraphicsContext *RendererInit::getContext() {
    switch (currentRendererApi) {
        case RendererApi::None:
            PND_CRITICAL("RENDERER API NOT SPECIFIED");
        case RendererApi::OpenGL:
#ifdef PND_PLATFORM_IOS
            return new GlesContext();
#else
            return new OpenGLContext();
#endif
    }
}

Renderer *RendererInit::getRenderer() {
    switch (currentRendererApi) {
        case RendererApi::None:
            PND_CRITICAL("RENDERER API NOT SPECIFIED");
        case RendererApi::OpenGL:
            return new OpenGLRenderer();
    }
}

Shader *RendererInit::createShader(const char *vertexPath, const char *fragmentPath) {
    switch (currentRendererApi) {
        case RendererApi::None:
            PND_CRITICAL("RENDERER API NOT SPECIFIED");
        case RendererApi::OpenGL:
            return new OpenGLShader(vertexPath, fragmentPath);
    }
}

Texture *RendererInit::createTexture(const char *path) {
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

VertexBuffer *RendererInit::createVertexBuffer(Vertex *vertices, unsigned int count, bool isDynamic) {
    switch (currentRendererApi) {
        case RendererApi::None:
            PND_CRITICAL("RENDERER API NOT SPECIFIED");
        case RendererApi::OpenGL:
            return new OpenGLVertexBuffer(vertices, count, isDynamic);
    };
}

VertexBuffer *RendererInit::createVertexBuffer(float *data, unsigned int count, bool isDynamic, VertexBufferLayout *layout) {
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
