//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Miren/VertexBufferLayoutData.hpp"

namespace Panda {

class OpenGLIndexBuffer {
public:
    OpenGLIndexBuffer(void *indices, BufferElementType elementType, size_t count, bool isDynamic);
    ~OpenGLIndexBuffer();
    void update(void *indices, size_t count);
    void bind() const;
    void unbind() const;
    uint32_t getCount() const;
    inline uint32_t getElementType() {
        return elementType;
    };

private:
    uint32_t id;
    uint32_t count;
    uint32_t elementType;
    size_t elementSize;
    bool isDynamic;
};

} // namespace Panda
