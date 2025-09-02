#include "Panda/Assets/MaterialAsset.hpp"
#include "Panda/Assets/MeshAsset.hpp"
#include "Panda/Assets/TextureAsset.hpp"
#include "Panda/Assets/ShaderAsset.hpp"
#include "Panda/Assets/Base/AssetHandler.hpp"

namespace Panda {

template<typename AssetType>
AssetRef<AssetType>::~AssetRef() {
    if (m_handler && m_id) { m_handler->decrementRefCount(m_id); }
}

template<typename AssetType>
AssetRef<AssetType>::AssetRef()
    : m_handler(nullptr)
    , m_id(0) {}

template<typename AssetType>
AssetRef<AssetType>::AssetRef(const AssetRef<AssetType> &other)
    : m_id(other.m_id)
    , m_handler(other.m_handler) {
    if (m_handler) { m_handler->incrementRefCount(m_id); }
}

template<typename AssetType>
AssetRef<AssetType>::AssetRef(AssetRef<AssetType> &&other)
    : m_id(other.m_id)
    , m_handler(other.m_handler) {
    other.m_id = 0;
    other.m_handler = nullptr;
}

template<typename AssetType>
AssetRef<AssetType>::AssetRef(AssetId id)
    : m_handler(GameContext::getAssetHandler())
    , m_id(id) {
    if (m_handler) { m_handler->incrementRefCount(m_id); }
}

template<typename AssetType>
AssetRef<AssetType>::AssetRef(AssetHandler *handler, AssetId id)
    : m_handler(handler)
    , m_id(id) {
    if (m_handler) { m_handler->incrementRefCount(m_id); }
}

template<typename AssetType>
Asset *AssetRef<AssetType>::get() const {
    PND_ASSERT(m_handler != nullptr, "INVALID ASSET REFERENCE");
    PND_ASSERT(m_id, "INVALID ASSET HANDLE");
    return m_handler->loadInternal(m_id);
}

template<typename AssetType>
AssetId AssetRef<AssetType>::getId() const {
    return m_id;
}

template<typename AssetType>
AssetRef<AssetType>::operator bool() const {
    return m_id;
}

template<typename AssetType>
AssetType *AssetRef<AssetType>::operator->() const {
    return dynamic_cast<AssetType *>(get());
}

template<typename AssetType>
AssetType &AssetRef<AssetType>::operator*() const {
    return *dynamic_cast<AssetType *>(get());
}

template<typename AssetType>
bool AssetRef<AssetType>::operator==(const AssetRef<AssetType> &other) const {
    return m_handler == other.m_handler && m_id == other.m_id;
}

template<typename AssetType>
AssetRef<AssetType> &AssetRef<AssetType>::operator=(const AssetRef<AssetType> &other) {
    if (this == &other) { return *this; }
    if (m_id && m_handler) { m_handler->decrementRefCount(m_id); }
    m_id = other.m_id;
    m_handler = other.m_handler;
    if (m_id && m_handler) { m_handler->incrementRefCount(m_id); }
    return *this;
}

template class AssetRef<Asset>;
template class AssetRef<ShaderAsset>;
template class AssetRef<MaterialAsset>;
template class AssetRef<TextureAsset>;
template class AssetRef<MeshAsset>;

} // namespace Panda