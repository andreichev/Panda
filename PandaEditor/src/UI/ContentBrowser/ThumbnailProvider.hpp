#pragma once

#include <Panda/Assets/TextureAsset.hpp>

namespace Panda {

class Thumbnail {
public:
    Thumbnail(Miren::TextureCreate create) {
        m_mirenHandle = Miren::createTexture(create);
        m_size = Size(create.m_width, create.m_height);
    }
    Thumbnail(Thumbnail &&other) {
        m_mirenHandle = other.m_mirenHandle;
        m_size = other.m_size;
        other.m_mirenHandle = MIREN_INVALID_HANDLE;
    }

    ~Thumbnail() {
        if (m_mirenHandle.isValid()) { Miren::deleteTexture(m_mirenHandle); }
    }

    Miren::TextureHandle getHandle() const {
        return m_mirenHandle;
    }

    Size getSize() const {
        return m_size;
    }

private:
    Miren::TextureHandle m_mirenHandle;
    Size m_size;
};

class ThumbnailProvider final {
public:
    Foundation::Shared<Thumbnail> getThumbnailOrNull(AssetId assetId, Size size);
    void resetCache();

private:
    std::unordered_map<AssetId, Foundation::Shared<Thumbnail>> m_cache;
};

} // namespace Panda