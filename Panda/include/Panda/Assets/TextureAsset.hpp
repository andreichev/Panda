#pragma once

#include "Panda/Base/Base.hpp"
#include "Base/Asset.hpp"
#include "Base/Path.hpp"
#include "Base/AssetImporterBase.hpp"

#include <Miren/Miren.hpp>
#include <cstdlib>

namespace Panda {

class TextureAsset : public Asset {
public:
    TextureAsset()
        : m_mirenHandle(MIREN_INVALID_HANDLE)
        , Asset(AssetType::TEXTURE) {}

    explicit TextureAsset(const path_t &path)
        : Asset(AssetType::TEXTURE) {
        auto textureCreate = AssetImporterBase::load2DTexture(path);
        textureCreate.m_wrap = Miren::CLAMP;
        textureCreate.m_magFiltering = Miren::NEAREST;
        textureCreate.m_minFiltering = Miren::NEAREST_MIPMAP_LINEAR;
        textureCreate.m_numMips = 3;
        create(textureCreate);
    }

    TextureAsset(Miren::TextureCreate textureCreate)
        : Asset(AssetType::TEXTURE) {
        create(textureCreate);
    }

    TextureAsset(TextureAsset &&other) {
        m_mirenHandle = other.m_mirenHandle;
        m_type = other.m_type;
        m_size = other.m_size;
        other.m_mirenHandle = MIREN_INVALID_HANDLE;
    }

    ~TextureAsset() {
        if (m_mirenHandle.isValid()) { Miren::deleteTexture(m_mirenHandle); }
    }

    void create(const Miren::TextureCreate &create) {
        m_mirenHandle = Miren::createTexture(create);
        m_size = Size(create.m_width, create.m_height);
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