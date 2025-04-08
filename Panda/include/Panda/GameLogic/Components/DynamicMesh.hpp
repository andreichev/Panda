//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Panda/Renderer/MeshData.hpp"
#include "Panda/Renderer/TextureBinding.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

class DynamicMesh {
public:
    ~DynamicMesh();
    DynamicMesh();

    void
    create(const MeshData &data, std::vector<TextureBinding> bindings, Miren::ProgramHandle shader);
    void update(const MeshData &data);

private:
    std::vector<TextureBinding> m_bindings;
    Miren::ProgramHandle m_shaderHandle;
    Miren::VertexLayoutHandle m_bufferLayoutHandle;
    Miren::IndexBufferHandle m_indexBufferHandle;
    Miren::VertexBufferHandle m_vertexBufferHandle;
    uint32_t m_indicesCount;

    friend class Renderer3D;
};

} // namespace Panda
