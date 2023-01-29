//
// Created by Admin on 12.02.2022.
//

#include "Chunk.hpp"

Chunk::~Chunk() {
    delete[] data;
}

Chunk::Chunk() {
    data = new Voxel[SIZE_X * SIZE_Y * SIZE_Z];
}

void Chunk::set(int x, int y, int z, char id) {
    if (x < 0 || y < 0 || z < 0 || x >= SIZE_X || y >= SIZE_Y || z >= SIZE_Z)
        return;
    data[y * Chunk::SIZE_X * Chunk::SIZE_Z + x * Chunk::SIZE_X + z].id = id;
}

Voxel *Chunk::get(int x, int y, int z) {
    if (x < 0 || y < 0 || z < 0 || x >= SIZE_X || y >= SIZE_Y || z >= SIZE_Z)
        return nullptr;
    return &data[y * Chunk::SIZE_X * Chunk::SIZE_Z + x * Chunk::SIZE_X + z];
}

Panda::Mesh *Chunk::getMesh() {
    return mesh;
}

void Chunk::setMesh(Panda::Mesh *mesh) {
    this->mesh = mesh;
}
