//
// Created by Admin on 12.02.2022.
//

#include "Chunk.hpp"

#include <Panda/Assets/MeshAsset.hpp>

Chunk::~Chunk() {
    delete[] m_data;
}

Chunk::Chunk() {
    m_data = new Voxel[SIZE_X * SIZE_Y * SIZE_Z];
}

void Chunk::set(int x, int y, int z, VoxelType type) {
    if (x < 0 || y < 0 || z < 0 || x >= SIZE_X || y >= SIZE_Y || z >= SIZE_Z) return;
    m_data[y * Chunk::SIZE_X * Chunk::SIZE_Z + x * Chunk::SIZE_X + z].type = type;
}

Voxel *Chunk::get(int x, int y, int z) {
    if (x < 0 || y < 0 || z < 0 || x >= SIZE_X || y >= SIZE_Y || z >= SIZE_Z) return nullptr;
    return &m_data[y * Chunk::SIZE_X * Chunk::SIZE_Z + x * Chunk::SIZE_X + z];
}

Panda::MeshAsset &Chunk::getMesh() {
    return m_mesh;
}