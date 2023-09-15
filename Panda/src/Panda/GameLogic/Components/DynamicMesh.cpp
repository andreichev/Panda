//
// Created by Admin on 12.02.2022.
//

#include "Panda/GameLogic/Components/DynamicMesh.hpp"

namespace Panda {

DynamicMesh::~DynamicMesh() {
    if (m_bufferLayoutHandle.isValid()) {
        Miren::deleteVertexLayout(m_bufferLayoutHandle);
    }
    if (m_vertexBufferHandle.isValid()) {
        Miren::deleteVertexBuffer(m_vertexBufferHandle);
    }
    if (m_indexBufferHandle.isValid()) {
        Miren::deleteIndexBuffer(m_indexBufferHandle);
    }
}

void DynamicMesh::create(
    const Panda::MeshData &data, Miren::TextureHandle texture, Miren::ProgramHandle shader) {
    PND_ASSERT(shader.isValid(), "Invalid shader for mesh");
    m_shaderHandle = shader;
    m_textureHandle = texture;
    m_indicesCount = data.indicesCount;
    Miren::VertexBufferLayoutData layoutData;
    layoutData.pushVector();
    m_bufferLayoutHandle = Miren::createVertexLayout(layoutData);
    m_vertexBufferHandle = Miren::createDynamicVertexBuffer(
        data.vertices, sizeof(Vertex) * data.verticesCount, m_bufferLayoutHandle);
    m_indexBufferHandle = Miren::createDynamicIndexBuffer(
        data.indices, Miren::BufferElementType::UnsignedInt, data.indicesCount);
}

void DynamicMesh::update(const MeshData &data) {
    PND_ASSERT(m_shaderHandle.isValid(), "Invalid shader for mesh");
    m_indicesCount = data.indicesCount;
    Miren::updateDynamicVertexBuffer(
        m_vertexBufferHandle, data.vertices, sizeof(Vertex) * data.verticesCount);
    Miren::updateDynamicIndexBuffer(m_indexBufferHandle, data.indices, m_indicesCount);
}

} // namespace Panda
