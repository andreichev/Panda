#pragma once

#include "AssetRef.hpp"
#include "Path.hpp"

#include <Miren/Base.hpp>
#include <string>

namespace Panda {

/// AssetHandler отвечает за загрузку Asset.
/// Для Editor - просто из файла, для Runtime - из AssetPack.
class AssetHandler {
protected:
    struct AssetEntry {
        Asset *asset;
        int refCount = 0;
        bool markedForDeletion = false;
    };

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
        AssetEntry entry;
        entry.asset = F_NEW(m_allocator, T)(std::forward<Args>(args)...);
        ;
        m_loadedAssets[id] = entry;
        return AssetRef<T>(this, id);
    }

    void cleanup() {
        auto it = m_loadedAssets.begin();
        while (it != m_loadedAssets.end()) {
            if (it->second.markedForDeletion && it->second.refCount <= 0) {
                AssetEntry &entry = it->second;
                F_FREE(m_allocator, entry.asset);
                it = m_loadedAssets.erase(it);
            } else {
                ++it;
            }
        }
    }

protected:
    virtual Asset *loadInternal(AssetId id, bool forcedReload = false) = 0;

    void incrementRefCount(AssetId id) {
        auto it = m_loadedAssets.find(id);
        if (it != m_loadedAssets.end()) {
            it->second.refCount++;
            it->second.markedForDeletion = false;
        }
    }

    void decrementRefCount(AssetId id) {
        auto it = m_loadedAssets.find(id);
        if (it != m_loadedAssets.end()) {
            it->second.refCount--;
            if (it->second.refCount <= 0) { it->second.markedForDeletion = true; }
        }
    }

    std::unordered_map<AssetId, AssetEntry> m_loadedAssets;
    Foundation::AllocatorI *m_allocator;

    template<typename T>
    friend class AssetRef;
};

} // namespace Panda
