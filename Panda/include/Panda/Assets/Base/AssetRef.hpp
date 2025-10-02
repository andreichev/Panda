#pragma once

#include "Asset.hpp"

#include <Foundation/Foundation.hpp>

namespace Panda {

class AssetHandler;

/// AssetRef stores only id and handler ref. It uses handler to access Asset by id.
template<typename AssetType = Asset>
class AssetRef final {
    static_assert(std::is_base_of_v<Asset, AssetType>);

public:
    ~AssetRef();
    AssetRef();
    AssetRef(const AssetRef &assetRef);
    AssetRef(AssetRef &&assetRef);
    AssetRef(AssetId id);
    AssetRef(AssetHandler *handler, AssetId id);
    Asset *get() const;
    AssetId getId() const;
    void reset();

    explicit operator bool() const;
    AssetType *operator->() const;
    AssetType &operator*() const;
    bool operator==(const AssetRef<AssetType> &other) const;
    AssetRef<AssetType> &operator=(const AssetRef<AssetType> &other);

    template<typename T>
    AssetRef<T> as() const {
        static_assert(std::is_base_of_v<AssetType, T>, "WRONG ASSET TYPE");
        return AssetRef<T>(m_handler, m_id);
    }

    AssetRef<Asset> asBaseAsset() const {
        return AssetRef<Asset>(m_handler, m_id);
    }

private:
    AssetHandler *m_handler;
    AssetId m_id;
};

} // namespace Panda
