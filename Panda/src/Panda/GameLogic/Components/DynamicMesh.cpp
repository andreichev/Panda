//
// Created by Admin on 12.02.2022.
//

#include "Panda/GameLogic/Components/DynamicMesh.hpp"

namespace Panda {

DynamicMesh::~DynamicMesh() {
    if (m_vertexBufferHandle.isValid()) {
        Miren::deleteVertexBuffer(m_vertexBufferHandle);
    }
    if (m_indexBufferHandle.isValid()) {
        Miren::deleteIndexBuffer(m_indexBufferHandle);
    }
}

DynamicMesh::DynamicMesh(DynamicMesh &&source)
    : m_model(source.m_model)
    , m_bufferLayoutHandle(source.m_bufferLayoutHandle)
    , m_indexBufferHandle(source.m_indexBufferHandle)
    , m_vertexBufferHandle(source.m_vertexBufferHandle)
    , m_indicesCount(source.m_indicesCount)
    , m_textureHandle(source.m_textureHandle)
    , m_shaderHandle(source.m_shaderHandle) {
    source.m_vertexBufferHandle = MIREN_INVALID_HANDLE;
    source.m_indexBufferHandle = MIREN_INVALID_HANDLE;
}

DynamicMesh::DynamicMesh()
    : m_model(1.f)
    , m_bufferLayoutHandle(MIREN_INVALID_HANDLE)
    , m_indexBufferHandle(MIREN_INVALID_HANDLE)
    , m_vertexBufferHandle(MIREN_INVALID_HANDLE)
    , m_indicesCount(0)
    , m_textureHandle(MIREN_INVALID_HANDLE)
    , m_shaderHandle(MIREN_INVALID_HANDLE) {}

DynamicMesh::DynamicMesh(DynamicMesh &source)
    : m_model(source.m_model)
    , m_bufferLayoutHandle(source.m_bufferLayoutHandle)
    , m_indexBufferHandle(source.m_indexBufferHandle)
    , m_vertexBufferHandle(source.m_vertexBufferHandle)
    , m_indicesCount(source.m_indicesCount)
    , m_textureHandle(source.m_textureHandle)
    , m_shaderHandle(source.m_shaderHandle) {}

void DynamicMesh::create(
    const Panda::MeshData &data, Miren::TextureHandle texture, Miren::ProgramHandle shader
) {
    PND_ASSERT(shader.isValid(), "Invalid shader for mesh");
    m_shaderHandle = shader;
    m_textureHandle = texture;
    m_indicesCount = data.indicesCount;
    m_bufferLayoutHandle = data.layoutHandle;
    m_vertexBufferHandle = Miren::createDynamicVertexBuffer(
        data.vertexBuffer, data.vertexBufferSize, m_bufferLayoutHandle
    );
    m_indexBufferHandle = Miren::createDynamicIndexBuffer(
        data.indices, Miren::BufferElementType::UnsignedInt, data.indicesCount
    );
}

void DynamicMesh::update(const MeshData &data) {
    PND_ASSERT(m_shaderHandle.isValid(), "Invalid shader for mesh");
    m_indicesCount = data.indicesCount;
    Miren::updateDynamicVertexBuffer(
        m_vertexBufferHandle, data.vertexBuffer, data.vertexBufferSize
    );
    Miren::updateDynamicIndexBuffer(m_indexBufferHandle, data.indices, m_indicesCount);
}

} // namespace Panda
