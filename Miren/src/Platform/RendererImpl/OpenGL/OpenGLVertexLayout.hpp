#pragma once

#include "Miren/VertexBufferLayoutData.hpp"

namespace Miren {

class OpenGLVertexLayout {
public:
    ~OpenGLVertexLayout();
    OpenGLVertexLayout(VertexBufferLayoutData *layoutData);
    void bind();
    void unbind();

private:
    uint32_t m_id;
};

} // namespace Miren