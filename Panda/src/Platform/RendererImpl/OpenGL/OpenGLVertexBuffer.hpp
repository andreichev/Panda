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
    OpenGLVertexBuffer(Vertex *data, unsigned int count, bool isDynamic);
    OpenGLVertexBuffer(float *data, unsigned int count, bool isDynamic, VertexBufferLayoutData *layout);
    void update(float *data, unsigned int count);
    void update(Vertex *data, unsigned int count);
    void bind();
    void unbind();

private:
    void createLayout(VertexBufferLayoutData* layoutData);
    unsigned int id;
    unsigned int layoutId;
    bool isDynamic;
};

} // namespace Panda