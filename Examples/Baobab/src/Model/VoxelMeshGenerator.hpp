//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Panda.hpp"
#include "ChunksStorage.hpp"

class VoxelMeshGenerator {
public:
    static Panda::MeshData makeOneChunkMesh(Miren::VertexLayoutHandle layoutHandle,
        ChunksStorage &chunks,
        int chunkIndexX,
        int chunkIndexY,
        int chunkIndexZ,
        bool ambientOcclusion);
    static void addFaceIndices(
        uint32_t offset, uint32_t &currentIndexNumber, unsigned int *indicesList);
    static bool isAir(int x, int y, int z, ChunksStorage &chunks);

private:
    static constexpr float ambientOcclusionFactor = 0.1f;
};
