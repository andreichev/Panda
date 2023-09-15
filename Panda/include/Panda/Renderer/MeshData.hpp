#pragma once

#include <glm/glm.hpp>

namespace Panda {

struct Vertex {
    glm::vec3 pos;
    glm::vec2 texCoords;
    float light;

    Vertex()
        : pos()
        , texCoords()
        , light(0.f) {}

    Vertex(const glm::vec3 &pos, const glm::vec2 &texCoords, float light)
        : pos(pos)
        , texCoords(texCoords)
        , light(light) {}

    Vertex(float x, float y, float z, float u, float v, float light)
        : pos(x, y, z)
        , texCoords(u, v)
        , light(light) {}
};

struct MeshData {
    Vertex *vertices;
    unsigned int verticesCount;
    unsigned int *indices;
    unsigned int indicesCount;

    MeshData(Vertex *vertices,
        unsigned int verticesCount,
        unsigned int *indices,
        unsigned int indicesCount)
        : vertices(vertices)
        , verticesCount(verticesCount)
        , indices(indices)
        , indicesCount(indicesCount) {}
};

} // namespace Panda