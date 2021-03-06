//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Panda/Renderer/Vertex.hpp"

namespace Panda {
struct MeshData {
    Vertex *vertices;
    unsigned int verticesCount;
    unsigned int *indices;
    unsigned int indicesCount;

    MeshData(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount)
        : vertices(vertices)
        , verticesCount(verticesCount)
        , indices(indices)
        , indicesCount(indicesCount) {}
};
} // namespace Panda
