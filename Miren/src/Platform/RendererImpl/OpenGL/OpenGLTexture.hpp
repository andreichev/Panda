//
// Created by Admin on 11.02.2022.
//

#pragma once

namespace Miren {

class OpenGLTexture {
public:
    OpenGLTexture();
    void create(const char *path);
    void create(void *pixels, uint32_t width, uint32_t height);
    void terminate();
    void bind(uint32_t slot);
    void unbind();

private:
    uint32_t m_id;
};

} // namespace Miren
