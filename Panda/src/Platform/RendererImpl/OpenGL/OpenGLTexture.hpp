//
// Created by Admin on 11.02.2022.
//

#pragma once

namespace Panda {

class OpenGLTexture {
public:
    ~OpenGLTexture();
    explicit OpenGLTexture(const char *path);
    void bind(unsigned int slot);
    void unbind();

private:
    unsigned int id;
};

} // namespace Panda
