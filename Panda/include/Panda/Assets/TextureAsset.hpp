#pragma once

#include "Panda/Base/Base.hpp"
#include "Base/AssetHandler.hpp"
#include "Base/AssetImporterBase.hpp"

#include <Miren/Miren.hpp>
#include <cstdlib>

namespace Panda {

class TextureAsset : public Asset {
public:
    TextureAsset()
        : m_mirenHandle(MIREN_INVALID_HANDLE) {}

    explicit TextureAsset(const path_t &path) {
        auto textureCreate = AssetImporterBase::load2DTexture(path);
        m_size = {(float)textureCreate.m_width, (float)textureCreate.m_height};
        textureCreate.m_wrap = Miren::CLAMP;
        textureCreate.m_magFiltering = Miren::NEAREST;
        textureCreate.m_minFiltering = Miren::NEAREST_MIPMAP_LINEAR;
        textureCreate.m_numMips = 3;
        m_mirenHandle = Miren::createTexture(textureCreate);
    }

    TextureAsset(Miren::TextureCreate create) {
        m_mirenHandle = Miren::createTexture(create);
        m_size = Size(create.m_width, create.m_height);
    }

    TextureAsset(TextureAsset &&other) {
        m_handle = other.m_handle;
        m_mirenHandle = other.m_mirenHandle;
        other.m_mirenHandle = MIREN_INVALID_HANDLE;
    }

    ~TextureAsset() {
        if (m_mirenHandle.isValid()) { Miren::deleteTexture(m_mirenHandle); }
    }

    Size getSize() const {
        return m_size;
    }

    inline Miren::TextureHandle getMirenHandle() {
        return m_mirenHandle;
    }

    bool operator==(const TextureAsset &o) {
        return o.m_mirenHandle.id == m_mirenHandle.id;
    }

private:
    Miren::TextureHandle m_mirenHandle;
    Size m_size;
};

} // namespace Panda