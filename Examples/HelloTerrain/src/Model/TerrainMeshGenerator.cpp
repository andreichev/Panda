//
// Created by Admin on 12.02.2022.
//

#include "TerrainMeshGenerator.hpp"
#include "Vertex.hpp"

#include <Panda.hpp>

void freeBuffer(void *data, void *userInfo) {
    FREE(Foundation::getAllocator(), data);
}

void initVertices(
    Vertex *vertices, uint32_t &verticesCount, int width, int depth, float *heightMap
) {
    float tileX = 10;
    float tileY = 10;
    float scale = 0.1f;
    for (int z = 0; z < depth; z++) {
        for (int x = 0; x < width; x++) {
            float h = heightMap[z * width + x] * 200.f;
            float u = x * tileX / (float)width;
            float v = z * tileY / (float)depth;
            vertices[verticesCount++] = Vertex(x * scale, h * scale, z * scale, u, v, h / 100.f);
        }
    }
}

void initIndices(uint32_t *indices, uint32_t &indicesCount, int width, int depth) {
    for (int z = 0; z < depth - 1; z++) {
        for (int x = 0; x < width - 1; x++) {
            unsigned int indexBottomLeft = z * width + x;
            unsigned int indexTopLeft = (z + 1) * width + x;
            unsigned int indexTopRight = (z + 1) * width + x + 1;
            unsigned int indexBottomRight = z * width + x + 1;

            indices[indicesCount++] = indexBottomLeft;
            indices[indicesCount++] = indexTopLeft;
            indices[indicesCount++] = indexTopRight;

            indices[indicesCount++] = indexBottomLeft;
            indices[indicesCount++] = indexTopRight;
            indices[indicesCount++] = indexBottomRight;
        }
    }
}

Panda::MeshData TerrainMeshGenerator::makeMesh(
    Miren::VertexLayoutHandle layoutHandle, int width, int depth, float *heightMap
) {
    using namespace Miren;
    using namespace Panda;
    Vertex *vertices = (Vertex *)ALLOC(Foundation::getAllocator(), sizeof(Vertex) * width * depth);
    uint32_t *indices =
        (uint32_t *)ALLOC(Foundation::getAllocator(), sizeof(uint32_t) * width * depth * 6);
    uint32_t verticesCount = 0;
    uint32_t indicesCount = 0;

    initVertices(vertices, verticesCount, width, depth, heightMap);
    initIndices(indices, indicesCount, width, depth);

    LOG_INFO("MESH GENERATED.");
    LOG_INFO("VERTICES COUNT: {}", verticesCount);
    LOG_INFO("INDICES COUNT: {}", indicesCount);
    Foundation::Memory verticesData = Foundation::Memory(vertices, nullptr, freeBuffer);
    Foundation::Memory indicesData = Foundation::Memory(indices, nullptr, freeBuffer);
    return MeshData(
        layoutHandle, verticesData, sizeof(Vertex) * verticesCount, indicesData, indicesCount
    );
}