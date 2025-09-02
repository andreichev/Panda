//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Panda/Assets/MaterialAsset.hpp"
#include "Panda/Renderer/MeshData.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

class MeshAsset : public Asset {
public:
    ~MeshAsset();
    MeshAsset();

    void create(const MeshData &data, AssetRef<MaterialAsset> material);
    void update(const MeshData &data);

    AssetRef<MaterialAsset> getMaterialAsset();

private:
    AssetRef<MaterialAsset> m_material;
    Miren::VertexLayoutHandle m_bufferLayoutHandle;
    Miren::IndexBufferHandle m_indexBufferHandle;
    Miren::VertexBufferHandle m_vertexBufferHandle;
    uint32_t m_indicesCount;

    friend class Renderer3D;
};

} // namespace Panda
