//
// Created by Admin on 12.02.2022.
//

#include "Panda/Assets/MeshAsset.hpp"

namespace Panda {

MeshAsset::~MeshAsset() {
    if (m_vertexBufferHandle.isValid()) { Miren::deleteVertexBuffer(m_vertexBufferHandle); }
    if (m_indexBufferHandle.isValid()) { Miren::deleteIndexBuffer(m_indexBufferHandle); }
}

MeshAsset::MeshAsset()
    : m_bufferLayoutHandle(MIREN_INVALID_HANDLE)
    , m_indexBufferHandle(MIREN_INVALID_HANDLE)
    , m_vertexBufferHandle(MIREN_INVALID_HANDLE)
    , m_indicesCount(0)
    , m_material() {}

void MeshAsset::create(const Panda::MeshData &data, Foundation::Shared<MaterialAsset> material) {
    PND_ASSERT(material && material->isValid(), "Invalid material for mesh");
    m_material = material;
    m_indicesCount = data.indicesCount;
    m_bufferLayoutHandle = data.layoutHandle;
    m_vertexBufferHandle = Miren::createDynamicVertexBuffer(
        data.vertexBuffer, data.vertexBufferSize, m_bufferLayoutHandle
    );
    m_indexBufferHandle = Miren::createDynamicIndexBuffer(
        data.indices, Miren::BufferElementType::UnsignedInt, data.indicesCount
    );
}

void MeshAsset::update(const MeshData &data) {
    PND_ASSERT(m_material && m_material->isValid(), "Invalid shader for mesh");
    m_indicesCount = data.indicesCount;
    Miren::updateDynamicVertexBuffer(
        m_vertexBufferHandle, data.vertexBuffer, data.vertexBufferSize
    );
    Miren::updateDynamicIndexBuffer(m_indexBufferHandle, data.indices, m_indicesCount);
}

Foundation::Shared<MaterialAsset> MeshAsset::getMaterialAsset() {
    return m_material;
}

} // namespace Panda
