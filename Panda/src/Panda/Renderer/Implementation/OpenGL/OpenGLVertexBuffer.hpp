//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Panda/Renderer/IVertexBuffer.hpp"
#include "Panda/Renderer/VertexBufferLayout.hpp"

namespace Panda {

class OpenGLVertexBuffer : public IVertexBuffer {
public:
    ~OpenGLVertexBuffer() override;
    OpenGLVertexBuffer(Vertex *data, unsigned int count, bool isDynamic);
    OpenGLVertexBuffer(float *data, unsigned int count, bool isDynamic, VertexBufferLayout *layout);
    void update(float *data, unsigned int count) override;
    void update(Vertex *data, unsigned int count) override;
    void bind() override;
    void unbind() override;

private:
    unsigned int id;
    VertexBufferLayout *layout;
    bool isDynamic;
};

} // namespace Panda