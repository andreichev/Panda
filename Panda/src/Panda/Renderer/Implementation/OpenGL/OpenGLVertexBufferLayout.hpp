//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Panda/Renderer/VertexBufferLayout.hpp"

namespace Panda {
class OpenGLVertexBufferLayout : public VertexBufferLayout {
public:
    ~OpenGLVertexBufferLayout() override;
    void initializeForRenderer() override;
    void bind() override;
    void unbind() override;

private:
    unsigned int id;
};
} // namespace Panda
