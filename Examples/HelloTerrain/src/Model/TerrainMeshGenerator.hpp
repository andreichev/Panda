//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Panda.hpp"

class TerrainMeshGenerator {
public:
    static Panda::MeshData
    makeMesh(Miren::VertexLayoutHandle layoutHandle, int width, int depth, float *heightMap);
};
