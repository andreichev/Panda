//
// Created by Admin on 12.02.2022.
//

#include "VoxelMeshGenerator.hpp"
#include "VoxelTextureMapper.hpp"
#include "Vertex.hpp"

#include <Panda.hpp>

void freeBuffer(void *data, void *userInfo) {
    FREE(Foundation::getAllocator(), data);
}

glm::vec2 getUV(uint8_t tileIndex) {
    // Размер одной текстуры на карте uv
    float uvSize = 1.f / 16.f;
    float u = tileIndex % 16 * uvSize;
    float v = tileIndex / 16 * uvSize;
    // Небольшой сдвиг от краев текстуры для того,
    // чтобы при mipmapping не было сливания с соседними текстурами
    u += 0.0005f;
    v += 0.0005f;
    return {u, v};
}

Panda::MeshData VoxelMeshGenerator::makeOneChunkMesh(
    Miren::VertexLayoutHandle layoutHandle,
    ChunksStorage &chunks,
    int chunkIndexX,
    int chunkIndexY,
    int chunkIndexZ,
    bool ambientOcclusion
) {
    using namespace Miren;
    using namespace Panda;
    Chunk &chunk = chunks.chunks
                       [chunkIndexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                        chunkIndexX * ChunksStorage::SIZE_X + chunkIndexZ];
    Vertex *vertices = (Vertex *)ALLOC(Foundation::getAllocator(), sizeof(Vertex) * 20000);
    uint32_t *indices = (uint32_t *)ALLOC(Foundation::getAllocator(), sizeof(uint32_t) * 20000);
    uint32_t verticesCount = 0;
    uint32_t indicesCount = 0;
    for (int voxelIndexX = 0; voxelIndexX < Chunk::SIZE_X; voxelIndexX++) {
        for (int voxelIndexY = 0; voxelIndexY < Chunk::SIZE_Y; voxelIndexY++) {
            for (int voxelIndexZ = 0; voxelIndexZ < Chunk::SIZE_Z; voxelIndexZ++) {
                int x = voxelIndexX + chunkIndexX * Chunk::SIZE_X;
                int y = voxelIndexY + chunkIndexY * Chunk::SIZE_Y;
                int z = voxelIndexZ + chunkIndexZ * Chunk::SIZE_Z;

                Voxel *currentVoxel = chunk.get(voxelIndexX, voxelIndexY, voxelIndexZ);
                if (currentVoxel == nullptr || currentVoxel->isAir()) {
                    continue;
                }
                VoxelTextureData &textureData = VoxelTextureMapper::getTextureData(currentVoxel);

                float uvSize = 1.f / 16.f;
                uvSize -= 0.001f;

                float light;
                float a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0;

                // Front
                if (isAir(x, y, z + 1, chunks)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    light = 1.0f;
                    if (ambientOcclusion) {
                        // top
                        a = (isAir(x, y + 1, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // bottom
                        b = (isAir(x, y - 1, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // left
                        c = (isAir(x + 1, y, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // right
                        d = (isAir(x - 1, y, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // top left
                        e = (isAir(x + 1, y + 1, z + 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // top right
                        f = (isAir(x - 1, y + 1, z + 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom left
                        g = (isAir(x + 1, y - 1, z + 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom right
                        h = (isAir(x - 1, y - 1, z + 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                    }
                    glm::vec2 uv = getUV(textureData.sideTileIndex);
                    vertices[verticesCount++] = Vertex(
                        x,
                        y,
                        z + 1.0f,
                        uv.x,
                        uv.y + uvSize,
                        textureData.sideColor,
                        light * (1.f - b - d - h)
                    );
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y,
                        z + 1.0f,
                        uv.x + uvSize,
                        uv.y + uvSize,
                        textureData.sideColor,
                        light * (1.f - b - c - g)
                    ); // 1
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y + 1.0f,
                        z + 1.0f,
                        uv.x + uvSize,
                        uv.y,
                        textureData.sideColor,
                        light * (1.f - a - c - e)
                    ); // 2
                    vertices[verticesCount++] = Vertex(
                        x,
                        y + 1.0f,
                        z + 1.0f,
                        uv.x,
                        uv.y,
                        textureData.sideColor,
                        light * (1.f - a - d - f)
                    ); // 3
                }
                // Back
                if (isAir(x, y, z - 1, chunks)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    light = 0.75f;
                    if (ambientOcclusion) {
                        // top
                        a = (isAir(x, y + 1, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // bottom
                        b = (isAir(x, y - 1, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // right
                        c = (isAir(x - 1, y, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // left
                        d = (isAir(x + 1, y, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // top right
                        e = (isAir(x - 1, y + 1, z - 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom right
                        f = (isAir(x - 1, y - 1, z - 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // top left
                        g = (isAir(x + 1, y + 1, z - 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom left
                        h = (isAir(x + 1, y - 1, z - 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                    }
                    glm::vec2 uv = getUV(textureData.sideTileIndex);
                    vertices[verticesCount++] = Vertex(
                        x,
                        y,
                        z,
                        uv.x + uvSize,
                        uv.y + uvSize,
                        textureData.sideColor,
                        light * (1.f - b - c - f)
                    ); // 4
                    vertices[verticesCount++] = Vertex(
                        x,
                        y + 1.0f,
                        z,
                        uv.x + uvSize,
                        uv.y,
                        textureData.sideColor,
                        light * (1.f - a - c - e)
                    ); // 5
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y + 1.0f,
                        z,
                        uv.x,
                        uv.y,
                        textureData.sideColor,
                        light * (1.f - a - d - g)
                    ); // 6
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y,
                        z,
                        uv.x,
                        uv.y + uvSize,
                        textureData.sideColor,
                        light * (1.f - b - d - h)
                    ); // 7
                }
                // Top
                if (isAir(x, y + 1, z, chunks)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    light = 0.95f;
                    if (ambientOcclusion) {
                        // left
                        a = (isAir(x + 1, y + 1, z, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // right
                        b = (isAir(x - 1, y + 1, z, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // back
                        c = (isAir(x, y + 1, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // front
                        d = (isAir(x, y + 1, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // left back
                        e = (isAir(x + 1, y + 1, z + 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // left front
                        f = (isAir(x + 1, y + 1, z - 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // right back
                        g = (isAir(x - 1, y + 1, z + 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // right front
                        h = (isAir(x - 1, y + 1, z - 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                    }
                    glm::vec2 uv = getUV(textureData.topTileIndex);
                    vertices[verticesCount++] = Vertex(
                        x,
                        y + 1.0f,
                        z,
                        uv.x,
                        uv.y + uvSize,
                        textureData.topColor,
                        light * (1.f - b - d - h)
                    ); // 8
                    vertices[verticesCount++] = Vertex(
                        x,
                        y + 1.0f,
                        z + 1.0f,
                        uv.x + uvSize,
                        uv.y + uvSize,
                        textureData.topColor,
                        light * (1.f - b - c - g)
                    ); // 11
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y + 1.0f,
                        z + 1.0f,
                        uv.x + uvSize,
                        uv.y,
                        textureData.topColor,
                        light * (1.f - a - c - e)
                    ); // 10
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y + 1.0f,
                        z,
                        uv.x,
                        uv.y,
                        textureData.topColor,
                        light * (1.f - a - d - f)
                    ); // 9
                }
                // Bottom
                if (isAir(x, y - 1, z, chunks)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    light = 0.85f;
                    if (ambientOcclusion) {
                        // left
                        a = (isAir(x + 1, y - 1, z, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // right
                        b = (isAir(x - 1, y - 1, z, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // back
                        c = (isAir(x, y - 1, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // front
                        d = (isAir(x, y - 1, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // left back
                        e = (isAir(x + 1, y - 1, z + 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // left front
                        f = (isAir(x + 1, y - 1, z - 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // right back
                        g = (isAir(x - 1, y - 1, z + 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // right front
                        h = (isAir(x - 1, y - 1, z - 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                    }
                    glm::vec2 uv = getUV(textureData.bottomTileIndex);
                    vertices[verticesCount++] = Vertex(
                        x,
                        y,
                        z,
                        uv.x,
                        uv.y + uvSize,
                        textureData.bottomColor,
                        light * (1.f - b - d - h)
                    ); // 12
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y,
                        z,
                        uv.x + uvSize,
                        uv.y + uvSize,
                        textureData.bottomColor,
                        light * (1.f - a - d - f)
                    ); // 13
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y,
                        z + 1.0f,
                        uv.x + uvSize,
                        uv.y,
                        textureData.bottomColor,
                        light * (1.f - a - c - e)
                    ); // 14
                    vertices[verticesCount++] = Vertex(
                        x,
                        y,
                        z + 1.0f,
                        uv.x,
                        uv.y,
                        textureData.bottomColor,
                        light * (1.f - b - c - g)
                    ); // 15
                }
                // Right
                if (isAir(x - 1, y, z, chunks)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    light = 0.9f;
                    if (ambientOcclusion) {
                        // top
                        a = (isAir(x - 1, y + 1, z, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // bottom
                        b = (isAir(x - 1, y - 1, z, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // front
                        c = (isAir(x - 1, y, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // back
                        d = (isAir(x - 1, y, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // top front
                        e = (isAir(x - 1, y + 1, z + 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom front
                        f = (isAir(x - 1, y - 1, z + 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // top back
                        g = (isAir(x - 1, y + 1, z - 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom back
                        h = (isAir(x - 1, y - 1, z - 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                    }
                    glm::vec2 uv = getUV(textureData.sideTileIndex);
                    vertices[verticesCount++] = Vertex(
                        x,
                        y,
                        z,
                        uv.x,
                        uv.y + uvSize,
                        textureData.sideColor,
                        light * (1.f - b - d - h)
                    ); // 16
                    vertices[verticesCount++] = Vertex(
                        x,
                        y,
                        z + 1.0f,
                        uv.x + uvSize,
                        uv.y + uvSize,
                        textureData.sideColor,
                        light * (1.f - b - c - f)
                    ); // 17
                    vertices[verticesCount++] = Vertex(
                        x,
                        y + 1.0f,
                        z + 1.0f,
                        uv.x + uvSize,
                        uv.y,
                        textureData.sideColor,
                        light * (1.f - a - c - e)
                    ); // 18
                    vertices[verticesCount++] = Vertex(
                        x,
                        y + 1.0f,
                        z,
                        uv.x,
                        uv.y,
                        textureData.sideColor,
                        light * (1.f - a - d - g)
                    ); // 19
                }
                // Left
                if (isAir(x + 1, y, z, chunks)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    light = 0.8f;
                    if (ambientOcclusion) {
                        // top
                        a = (isAir(x + 1, y + 1, z, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // bottom
                        b = (isAir(x + 1, y - 1, z, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // front
                        c = (isAir(x + 1, y, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // back
                        d = (isAir(x + 1, y, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // top front
                        e = (isAir(x + 1, y + 1, z + 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // top back
                        f = (isAir(x + 1, y + 1, z - 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom front
                        g = (isAir(x + 1, y - 1, z + 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                        // bottom back
                        h = (isAir(x + 1, y - 1, z - 1, chunks) ? 0.0f : 1.0f) *
                            ambientOcclusionFactor;
                    }
                    glm::vec2 uv = getUV(textureData.sideTileIndex);
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y,
                        z,
                        uv.x + uvSize,
                        uv.y + uvSize,
                        textureData.sideColor,
                        light * (1.f - b - d - h)
                    ); // 20
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y + 1.0f,
                        z,
                        uv.x + uvSize,
                        uv.y,
                        textureData.sideColor,
                        light * (1.f - a - d - f)
                    ); // 23
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y + 1.0f,
                        z + 1.0f,
                        uv.x,
                        uv.y,
                        textureData.sideColor,
                        light * (1.f - a - c - e)
                    ); // 22
                    vertices[verticesCount++] = Vertex(
                        x + 1.0f,
                        y,
                        z + 1.0f,
                        uv.x,
                        uv.y + uvSize,
                        textureData.sideColor,
                        light * (1.f - b - c - g)
                    ); // 21
                }
            }
        }
    }
    // LOG_INFO("MESH GENERATED.");
    // LOG_INFO("VERTICES COUNT: {}", verticesCount);
    // LOG_INFO("INDICES COUNT: {}", indicesCount);
    Foundation::Memory verticesData = Foundation::Memory(vertices, nullptr, freeBuffer);
    Foundation::Memory indicesData = Foundation::Memory(indices, nullptr, freeBuffer);
    return MeshData(
        layoutHandle, verticesData, sizeof(Vertex) * verticesCount, indicesData, indicesCount
    );
}

void VoxelMeshGenerator::addFaceIndices(
    uint32_t offset, uint32_t &indicesCount, unsigned int *indices
) {
    indices[indicesCount++] = offset;
    indices[indicesCount++] = offset + 1;
    indices[indicesCount++] = offset + 2;
    indices[indicesCount++] = offset + 2;
    indices[indicesCount++] = offset + 3;
    indices[indicesCount++] = offset;
}

inline bool VoxelMeshGenerator::isAir(int x, int y, int z, ChunksStorage &chunks) {
    Voxel *voxel = chunks.getVoxel(x, y, z);
    if (voxel == nullptr) {
        return true;
    }
    return voxel->isAir();
}
