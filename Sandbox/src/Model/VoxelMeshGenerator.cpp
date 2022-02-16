//
// Created by Admin on 12.02.2022.
//
#include "pndpch.hpp"
#include "VoxelMeshGenerator.hpp"

#include <Panda/Renderer/Vertex.hpp>

Panda::MeshData VoxelMeshGenerator::makeOneChunkMesh(
    ChunksStorage &chunks, int chunkIndexX, int chunkIndexY, int chunkIndexZ, bool ambientOcclusion) {
    Chunk &chunk =
        chunks.chunks[chunkIndexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z + chunkIndexX * ChunksStorage::SIZE_X + chunkIndexZ];
    std::vector<Vertex> &verticesList = *new std::vector<Vertex>;
    std::vector<unsigned int> &indicesList = *new std::vector<unsigned int>;
    for (int voxelIndexX = 0; voxelIndexX < Chunk::SIZE_X; voxelIndexX++) {
        for (int voxelIndexY = 0; voxelIndexY < Chunk::SIZE_Y; voxelIndexY++) {
            for (int voxelIndexZ = 0; voxelIndexZ < Chunk::SIZE_Z; voxelIndexZ++) {
                int x = voxelIndexX + chunkIndexX * Chunk::SIZE_X;
                int y = voxelIndexY + chunkIndexY * Chunk::SIZE_Y;
                int z = voxelIndexZ + chunkIndexZ * Chunk::SIZE_Z;

                Voxel *currentVoxel = chunk.get(voxelIndexX, voxelIndexY, voxelIndexZ);
                if (currentVoxel == nullptr || currentVoxel->id == 0) {
                    continue;
                }

                // Размер одной текстуры на карте uv
                float uvSize = 1.f / 16.f;
                float u = currentVoxel->id % 16 * uvSize;
                float v = currentVoxel->id / 16 * uvSize;

                // Небольшой сдвиг от краев текстуры для того,
                // чтобы при mipmapping не было сливания с соседними текстурами
                uvSize -= 0.001f;
                u += 0.0005f;
                v += 0.0005f;

                float light;
                float a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0;

                // Front
                if (isAir(x, y, z + 1, chunks)) {
                    addFaceIndices(verticesList.size(), indicesList);
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
                        e = (isAir(x + 1, y + 1, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // top right
                        f = (isAir(x - 1, y + 1, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // bottom left
                        g = (isAir(x + 1, y - 1, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // bottom right
                        h = (isAir(x - 1, y - 1, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                    }
                    verticesList.emplace_back(x, y, z + 1.0f, u, v + uvSize, light * (1.f - b - d - h));
                    verticesList.emplace_back(x + 1.0f, y, z + 1.0f, u + uvSize, v + uvSize, light * (1.f - b - c - g)); // 1
                    verticesList.emplace_back(x + 1.0f, y + 1.0f, z + 1.0f, u + uvSize, v, light * (1.f - a - c - e));   // 2
                    verticesList.emplace_back(x, y + 1.0f, z + 1.0f, u, v, light * (1.f - a - d - f));                   // 3
                }
                // Back
                if (isAir(x, y, z - 1, chunks)) {
                    addFaceIndices(verticesList.size(), indicesList);
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
                        e = (isAir(x - 1, y + 1, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // bottom right
                        f = (isAir(x - 1, y - 1, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // top left
                        g = (isAir(x + 1, y + 1, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // bottom left
                        h = (isAir(x + 1, y - 1, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                    }
                    verticesList.emplace_back(x, y, z, u + uvSize, v + uvSize, light * (1.f - b - c - f)); // 4
                    verticesList.emplace_back(x, y + 1.0f, z, u + uvSize, v, light * (1.f - a - c - e));   // 5
                    verticesList.emplace_back(x + 1.0f, y + 1.0f, z, u, v, light * (1.f - a - d - g));     // 6
                    verticesList.emplace_back(x + 1.0f, y, z, u, v + uvSize, light * (1.f - b - d - h));   // 7
                }
                // Top
                if (isAir(x, y + 1, z, chunks)) {
                    addFaceIndices(verticesList.size(), indicesList);
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
                        e = (isAir(x + 1, y + 1, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // left front
                        f = (isAir(x + 1, y + 1, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // right back
                        g = (isAir(x - 1, y + 1, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // right front
                        h = (isAir(x - 1, y + 1, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                    }
                    verticesList.emplace_back(x, y + 1.0f, z, u, v + uvSize, light * (1.f - b - d - h));                 // 8
                    verticesList.emplace_back(x, y + 1.0f, z + 1.0f, u + uvSize, v + uvSize, light * (1.f - b - c - g)); // 11
                    verticesList.emplace_back(x + 1.0f, y + 1.0f, z + 1.0f, u + uvSize, v, light * (1.f - a - c - e));   // 10
                    verticesList.emplace_back(x + 1.0f, y + 1.0f, z, u, v, light * (1.f - a - d - f));                   // 9
                }
                // Bottom
                if (isAir(x, y - 1, z, chunks)) {
                    addFaceIndices(verticesList.size(), indicesList);
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
                        e = (isAir(x + 1, y - 1, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // left front
                        f = (isAir(x + 1, y - 1, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // right back
                        g = (isAir(x - 1, y - 1, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // right front
                        h = (isAir(x - 1, y - 1, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                    }
                    verticesList.emplace_back(x, y, z, u, v + uvSize, light * (1.f - b - d - h));                 // 12
                    verticesList.emplace_back(x + 1.0f, y, z, u + uvSize, v + uvSize, light * (1.f - a - d - f)); // 13
                    verticesList.emplace_back(x + 1.0f, y, z + 1.0f, u + uvSize, v, light * (1.f - a - c - e));   // 14
                    verticesList.emplace_back(x, y, z + 1.0f, u, v, light * (1.f - b - c - g));                   // 15
                }
                // Right
                if (isAir(x - 1, y, z, chunks)) {
                    addFaceIndices(verticesList.size(), indicesList);
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
                        e = (isAir(x - 1, y + 1, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // bottom front
                        f = (isAir(x - 1, y - 1, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // top back
                        g = (isAir(x - 1, y + 1, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // bottom back
                        h = (isAir(x - 1, y - 1, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                    }
                    verticesList.emplace_back(x, y, z, u, v + uvSize, light * (1.f - b - d - h));                 // 16
                    verticesList.emplace_back(x, y, z + 1.0f, u + uvSize, v + uvSize, light * (1.f - b - c - f)); // 17
                    verticesList.emplace_back(x, y + 1.0f, z + 1.0f, u + uvSize, v, light * (1.f - a - c - e));   // 18
                    verticesList.emplace_back(x, y + 1.0f, z, u, v, light * (1.f - a - d - g));                   // 19
                }
                // Left
                if (isAir(x + 1, y, z, chunks)) {
                    addFaceIndices(verticesList.size(), indicesList);
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
                        e = (isAir(x + 1, y + 1, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // top back
                        f = (isAir(x + 1, y + 1, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // bottom front
                        g = (isAir(x + 1, y - 1, z + 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                        // bottom back
                        h = (isAir(x + 1, y - 1, z - 1, chunks) ? 0.0f : 1.0f) * ambientOcclusionFactor;
                    }
                    verticesList.emplace_back(x + 1.0f, y, z, u + uvSize, v + uvSize, light * (1.f - b - d - h)); // 20
                    verticesList.emplace_back(x + 1.0f, y + 1.0f, z, u + uvSize, v, light * (1.f - a - d - f));   // 23
                    verticesList.emplace_back(x + 1.0f, y + 1.0f, z + 1.0f, u, v, light * (1.f - a - c - e));     // 22
                    verticesList.emplace_back(x + 1.0f, y, z + 1.0f, u, v + uvSize, light * (1.f - b - c - g));   // 21
                }
            }
        }
    }
    return Panda::MeshData(&verticesList[0], verticesList.size(), &indicesList[0], indicesList.size());
}

void VoxelMeshGenerator::addFaceIndices(int offset, std::vector<unsigned int> &indicesList) {
    indicesList.emplace_back(offset);
    indicesList.emplace_back(offset + 1);
    indicesList.emplace_back(offset + 2);
    indicesList.emplace_back(offset + 2);
    indicesList.emplace_back(offset + 3);
    indicesList.emplace_back(offset);
}

bool VoxelMeshGenerator::isAir(int x, int y, int z, ChunksStorage &chunks) {
    Voxel *voxel = chunks.getVoxel(x, y, z);
    if (voxel == nullptr) {
        return true;
    }
    return voxel->id == 0;
}
