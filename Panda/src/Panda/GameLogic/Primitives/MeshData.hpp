//
// Created by Admin on 12.02.2022.
//

#pragma once

#include <Miren/Vertex.hpp>

namespace Panda {
struct MeshData {
    Miren::Vertex *vertices;
    unsigned int verticesCount;
    unsigned int *indices;
    unsigned int indicesCount;

    MeshData(Miren::Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount)
        : vertices(vertices)
        , verticesCount(verticesCount)
        , indices(indices)
        , indicesCount(indicesCount) {}
};
} // namespace Panda
