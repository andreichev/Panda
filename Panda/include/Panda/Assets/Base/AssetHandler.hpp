#pragma once

#include "AssetRef.hpp"
#include "Path.hpp"

#include <Miren/Base.hpp>
#include <string>

namespace Panda {

/// AssetHandler отвечает за загрузку Asset.
/// Для Editor - просто из файла, для Runtime - из AssetPack.
class AssetHandler {
public:
    virtual ~AssetHandler() = default;

    AssetHandler(Foundation::AllocatorI *allocator)
        : m_allocator(allocator)
        , m_loadedAssets() {}

    template<typename T = Asset>
    AssetRef<T> makeRef(AssetId id) {
        return AssetRef<T>(this, id);
    }

    template<typename T, typename... Args>
    AssetRef<T> createStaticAsset(AssetId id, Args &&...args) {
        T *asset = F_NEW(m_allocator, T)(std::forward<Args>(args)...);
        m_loadedAssets[id] = asset;
        return AssetRef<T>(this, id);
    }

protected:
    virtual Asset *loadInternal(AssetId id) = 0;
    virtual void incrementRefCount(AssetId id) {}
    virtual void decrementRefCount(AssetId id) {}

    std::unordered_map<AssetId, Asset *> m_loadedAssets;
    Foundation::AllocatorI *m_allocator;

    template<typename T>
    friend class AssetRef;
};

} // namespace Panda
