//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Voxel.hpp"
#include "Panda.hpp"

class Chunk {
public:
    static const int SIZE_X = 20;
    static const int SIZE_Y = 20;
    static const int SIZE_Z = 20;

    Chunk();
    ~Chunk();
    Panda::Mesh *getMesh();
    void setMesh(Panda::Mesh *mesh);
    void set(int x, int y, int z, char id);
    Voxel *get(int x, int y, int z);
    Voxel *m_data;

private:
    Panda::Mesh *m_mesh;
};
