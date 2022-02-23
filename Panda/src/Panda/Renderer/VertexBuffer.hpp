//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Vertex.hpp"

namespace Panda {
class VertexBuffer {
public:
    virtual ~VertexBuffer() = default;
    virtual void update(float *data, unsigned int count) = 0;
    virtual void update(Vertex *data, unsigned int count) = 0;
    virtual void bind() = 0;
    virtual void unbind() = 0;
};
} // namespace Panda
