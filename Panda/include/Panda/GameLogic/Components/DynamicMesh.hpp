//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Panda/Renderer/MeshData.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

class DynamicMesh {
public:
    ~DynamicMesh();
    DynamicMesh();
    DynamicMesh(DynamicMesh &&source);
    DynamicMesh(DynamicMesh &source);

    void create(const MeshData &data, Miren::TextureHandle texture, Miren::ProgramHandle shader);
    void update(const MeshData &data);

private:
    Miren::TextureHandle m_textureHandle;
    Miren::ProgramHandle m_shaderHandle;
    Miren::VertexLayoutHandle m_bufferLayoutHandle;
    Miren::IndexBufferHandle m_indexBufferHandle;
    Miren::VertexBufferHandle m_vertexBufferHandle;
    glm::mat4 m_model;
    uint32_t m_indicesCount;

    friend class Renderer3D;
};

} // namespace Panda
