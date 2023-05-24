#pragma once

#include "Panda/Assets/AssetLoader.hpp"

#include <Miren/Miren.hpp>
#include <cstdlib>

namespace Panda {
class Texture {
public:
    Texture(const char *path) {
        TextureAsset asset = AssetLoader::loadTexure(path);
        m_handle = Miren::createTexture(asset.getMirenTextureCreate());
        LOG_INFO("CREATED TEXTURE, path: {}", path);
    }

    Texture(uint8_t *data, uint32_t width, uint32_t height) {
        Miren::TextureCreate create;
        create.m_data = Foundation::Memory(data);
        create.m_format = Miren::TextureFormat::RGBA8;
        create.m_numMips = 0;
        create.m_width = width;
        create.m_height = height;
        m_handle = Miren::createTexture(create);
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