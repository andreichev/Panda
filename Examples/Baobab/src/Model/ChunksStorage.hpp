//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Chunk.hpp"
#include "VoxelRaycastData.hpp"

#include <optional>

class ChunksStorage {
public:
    static const int SIZE_X = 10;
    static const int SIZE_Y = 4;
    static const int SIZE_Z = 10;

    static const int WORLD_SIZE_X = ChunksStorage::SIZE_X * Chunk::SIZE_X;
    static const int WORLD_SIZE_Y = ChunksStorage::SIZE_Y * Chunk::SIZE_Y;
    static const int WORLD_SIZE_Z = ChunksStorage::SIZE_Z * Chunk::SIZE_Z;

    Chunk *chunks;

    ChunksStorage();
    ~ChunksStorage();

    void setVoxel(int x, int y, int z, uint8_t id);
    Voxel *getVoxel(int x, int y, int z);
    Chunk *getChunk(int x, int y, int z);
    std::optional<VoxelRaycastData> bresenham3D(
        float x1, float y1, float z1, float x2, float y2, float z2, int maximumDistance);
};
