#pragma once

#include "Panda/Base/Base.hpp"
#include "Panda/Assets/AssetHandler.hpp"
#include "Panda/Assets/AssetLoaderEditor.hpp"

#include <Miren/Miren.hpp>
#include <cstdlib>

namespace Panda {

class Texture : public Asset {
public:
    Texture()
        : m_mirenHandle(MIREN_INVALID_HANDLE) {}

    /// Editor constructor
    explicit Texture(const path_t &path) {
        TextureData data = AssetLoaderEditor::loadTexture(path);
        m_size = {(float)data.m_width, (float)data.m_height};
        auto textureCreate = data.getMirenTextureCreate();
        textureCreate.m_wrap = Miren::CLAMP;
        textureCreate.m_magFiltering = Miren::NEAREST;
        textureCreate.m_minFiltering = Miren::NEAREST_MIPMAP_LINEAR;
        textureCreate.m_numMips = 3;
        m_mirenHandle = Miren::createTexture(textureCreate);
        LOG_INFO("CREATED TEXTURE, path: %s", path.string().c_str());
    }

    /// Runtime constructor
    Texture(
        Foundation::Memory mem,
        uint32_t width,
        uint32_t height,
        Miren::TextureFormat format = Miren::RGBA8,
        int numMips = 0
    ) {
        Miren::TextureCreate create;
        create.m_data = mem;
        create.m_format = format;
        create.m_numMips = 0;
        create.m_width = width;
        create.m_height = height;
        m_size = {(float)width, (float)height};
        m_mirenHandle = Miren::createTexture(create);
        LOG_INFO("CREATED TEXTURE, w: %d, h: %d", width, height);
    }

    Texture(Texture &&other) {
        m_handle = other.m_handle;
        m_mirenHandle = other.m_mirenHandle;
        other.m_mirenHandle = MIREN_INVALID_HANDLE;
        LOG_INFO("MOVED TEXTURE");
    }

    ~Texture() {
        if (m_mirenHandle.isValid()) {
            Miren::deleteTexture(m_mirenHandle);
            LOG_INFO("DELETED TEXTURE");
        }
    }

    Size getSize() const {
        return m_size;
    }

    inline Miren::TextureHandle getMirenHandle() {
        return m_mirenHandle;
    }

    bool operator==(const Texture &o) {
        return o.m_mirenHandle.id == m_mirenHandle.id;
    }

private:
    Miren::TextureHandle m_mirenHandle;
    Size m_size;
};

} // namespace Panda