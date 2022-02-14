//
// Created by Admin on 11.02.2022.
//

#pragma once

namespace Panda {
class IndexBuffer {
public:
    virtual ~IndexBuffer() = default;
    virtual void update(unsigned int *indices) = 0;
    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    virtual unsigned int getSize() const = 0;
};
} // namespace Panda
