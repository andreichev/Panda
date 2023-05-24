//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Miren/Base.hpp"

namespace Miren {

class OpenGLTexture {
public:
    OpenGLTexture();
    void create(const TextureCreate &create);
    void terminate();
    void bind(uint32_t slot);
    void unbind();
    inline uint32_t getId() {
        return m_id;
    }

    inline TextureFormat getFormat() {
        return m_format;
    }

private:
    TextureFormat m_format;
    uint32_t m_id;
};

} // namespace Miren
