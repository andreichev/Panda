//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Miren/Base.hpp"

namespace Miren {

class OpenGLTexture {
public:
    OpenGLTexture();
    void create(TextureCreate &create);
    void update(Foundation::Memory mem);
    void resize(uint32_t width, uint32_t height);

    uint16_t getWidth() {
        return m_create.m_width;
    }

    uint16_t getHeight() {
        return m_create.m_height;
    }
    void terminate();

    inline uint32_t getId() {
        return m_id;
    }

    inline TextureFormat getFormat() {
        return m_create.m_format;
    }

    void readPixels(void *data);

private:
    TextureCreate m_create;
    uint32_t m_id;
    uint32_t m_target;
    uint32_t m_format;
    uint32_t m_type;
};

} // namespace Miren
