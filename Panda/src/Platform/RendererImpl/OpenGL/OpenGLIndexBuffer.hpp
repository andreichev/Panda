//
// Created by Admin on 11.02.2022.
//

#pragma once

namespace Panda {

class OpenGLIndexBuffer {
public:
    OpenGLIndexBuffer(uint32_t *indices, uint32_t count, bool isDynamic);
    ~OpenGLIndexBuffer();
    void update(uint32_t *indices, uint32_t count);
    void bind() const;
    void unbind() const;
    uint32_t getSize() const;

private:
    uint32_t id;
    uint32_t size;
    bool isDynamic;
};

} // namespace Panda
