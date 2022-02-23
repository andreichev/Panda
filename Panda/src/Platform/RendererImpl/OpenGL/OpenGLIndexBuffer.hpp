//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Panda/Renderer/IndexBuffer.hpp"

namespace Panda {

class OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(unsigned int *indices, unsigned int count, bool isDynamic);
    ~OpenGLIndexBuffer() override;

private:
    void update(unsigned int *indices) override;
    void bind() const override;
    void unbind() const override;
    unsigned int getSize() const override;

private:
    unsigned int id;
    unsigned int size;
    bool isDynamic;
};

} // namespace Panda
