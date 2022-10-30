//
// Created by Admin on 11.02.2022.
//

#pragma once

namespace Miren {

class OpenGLTexture {
public:
    ~OpenGLTexture();
    explicit OpenGLTexture(const char *path);
    explicit OpenGLTexture(void *pixels, int width, int height);
    void bind(unsigned int slot);
    void unbind();

private:
    unsigned int id;
};

} // namespace Miren
