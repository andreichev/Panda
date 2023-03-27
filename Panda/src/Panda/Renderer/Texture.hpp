#pragma once

#include <Miren/Miren.hpp>

namespace Panda {
class Texture {
public:
    Texture(const char *path) {
        m_handle = Miren::createTextureFromFile(path);
        LOG_INFO("CREATED TEXTURE, path: {}", path);
    }
    Texture(uint8_t *data, uint32_t width, u_int32_t height) {
        m_handle = Miren::createTextureFromPixels(data, width, height);
        LOG_INFO("CREATED TEXTURE, w: {}, h: {}", width, height);
    }
    ~Texture() {
        Miren::deleteTexture(m_handle);
        LOG_INFO("DELETED TEXTURE");
    }
    inline Miren::TextureHandle getHandle() {
        return m_handle;
    }

    bool operator==(const Texture &o) {
        return o.m_handle == m_handle;
    }

private:
    Miren::TextureHandle m_handle;
};
} // namespace Panda