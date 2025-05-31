//
// Created by Admin on 12.02.2022.
//

#include "Panda/Assets/DynamicMeshAsset.hpp"

namespace Panda {

DynamicMeshAsset::~DynamicMeshAsset() {
    if (m_vertexBufferHandle.isValid()) { Miren::deleteVertexBuffer(m_vertexBufferHandle); }
    if (m_indexBufferHandle.isValid()) { Miren::deleteIndexBuffer(m_indexBufferHandle); }
}

DynamicMeshAsset::DynamicMeshAsset()
    : m_bufferLayoutHandle(MIREN_INVALID_HANDLE)
    , m_indexBufferHandle(MIREN_INVALID_HANDLE)
    , m_vertexBufferHandle(MIREN_INVALID_HANDLE)
    , m_indicesCount(0)
    , m_bindings()
    , m_shaderHandle(MIREN_INVALID_HANDLE) {}

void DynamicMeshAsset::create(
    const Panda::MeshData &data, std::vector<TextureBinding> bindings, Miren::ProgramHandle shader
) {
    PND_ASSERT(shader.isValid(), "Invalid shader for mesh");
    m_shaderHandle = shader;
    m_bindings = bindings;
    m_indicesCount = data.indicesCount;
    m_bufferLayoutHandle = data.layoutHandle;
    m_vertexBufferHandle = Miren::createDynamicVertexBuffer(
        data.vertexBuffer, data.vertexBufferSize, m_bufferLayoutHandle
    );
    m_indexBufferHandle = Miren::createDynamicIndexBuffer(
        data.indices, Miren::BufferElementType::UnsignedInt, data.indicesCount
    );
}

void DynamicMeshAsset::update(const MeshData &data) {
    PND_ASSERT(m_shaderHandle.isValid(), "Invalid shader for mesh");
    m_indicesCount = data.indicesCount;
    Miren::updateDynamicVertexBuffer(
        m_vertexBufferHandle, data.vertexBuffer, data.vertexBufferSize
    );
    Miren::updateDynamicIndexBuffer(m_indexBufferHandle, data.indices, m_indicesCount);
}

} // namespace Panda
