//
// Created by Admin on 11.02.2022.
//

#pragma once

namespace Panda {

class OpenGLIndexBuffer {
public:
    OpenGLIndexBuffer(unsigned int *indices, unsigned int count, bool isDynamic);
    ~OpenGLIndexBuffer();
    void update(unsigned int *indices);
    void bind() const;
    void unbind() const;
    unsigned int getSize() const;

private:
    unsigned int id;
    unsigned int size;
    bool isDynamic;
};

} // namespace Panda
