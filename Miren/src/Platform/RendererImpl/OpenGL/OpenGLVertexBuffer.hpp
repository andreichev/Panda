//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Miren/VertexBufferLayoutData.hpp"
#include "Miren/Base.hpp"

namespace Miren {

class OpenGLVertexBuffer {
public:
    ~OpenGLVertexBuffer();
    OpenGLVertexBuffer(void *data, uint32_t size, bool isDynamic);
    void update(void *data, uint32_t size);
    void bind();
    void unbind();
    void setLayoutHandle(VertexLayoutHandle layoutHandle);
    VertexLayoutHandle getLayoutHandle();

private:
    uint32_t id;
    bool isDynamic;
    VertexLayoutHandle m_layoutHandle;
};

} // namespace Miren