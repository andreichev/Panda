//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Panda/Renderer/Vertex.hpp"
#include "Panda/Renderer/VertexBufferLayoutData.hpp"

namespace Panda {

class OpenGLVertexBuffer {
public:
    ~OpenGLVertexBuffer();
    OpenGLVertexBuffer(void *data, uint32_t size, bool isDynamic, VertexBufferLayoutData *layout);
    void update(void *data, uint32_t size);
    void bind();
    void unbind();

private:
    void createLayout(VertexBufferLayoutData *layoutData);
    uint32_t id;
    uint32_t layoutId;
    bool isDynamic;
};

} // namespace Panda