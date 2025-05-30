//
// Created by Admin on 12.02.2022.
//

#include "Panda/Assets/StaticMeshAsset.hpp"

namespace Panda {

StaticMeshAsset::~StaticMeshAsset() {
    if (m_vertexBufferHandle.isValid()) { Miren::deleteVertexBuffer(m_vertexBufferHandle); }
    if (m_indexBufferHandle.isValid()) { Miren::deleteIndexBuffer(m_indexBufferHandle); }
    if (m_bufferLayoutHandle.isValid()) { deleteVertexLayout(m_bufferLayoutHandle); }
}

void StaticMeshAsset::create(
    const Panda::MeshData &data, Miren::TextureHandle texture, Miren::ProgramHandle shader
) {
    m_shaderHandle = shader;
    m_textureHandle = texture;
    m_indicesCount = data.indicesCount;
    m_bufferLayoutHandle = data.layoutHandle;
    m_vertexBufferHandle =
        Miren::createVertexBuffer(data.vertexBuffer, data.vertexBufferSize, m_bufferLayoutHandle);
    m_indexBufferHandle = Miren::createIndexBuffer(
        data.indices, Miren::BufferElementType::UnsignedInt, data.indicesCount
    );
}

} // namespace Panda
