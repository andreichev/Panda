//
// Created by Admin on 12.02.2022.
//

#include "ChunksStorage.hpp"
#include "PerlinNoise.hpp"
#include "Voxel.hpp"

ChunksStorage::~ChunksStorage() {
    delete[] chunks;
}

ChunksStorage::ChunksStorage() {
    LOG_INFO("NOISE GENERATION STARTED, number of chunks: %d", SIZE_X * SIZE_Y * SIZE_Z);
    chunks = new Chunk[SIZE_X * SIZE_Y * SIZE_Z];

    float terrain[WORLD_SIZE_X * WORLD_SIZE_Z];
    PerlinNoise::generate2DGlm(2, 4, 1.0f, terrain, WORLD_SIZE_X, WORLD_SIZE_Z);

    for (int x = 0; x < WORLD_SIZE_X; x++) {
        for (int y = 0; y < WORLD_SIZE_Y; y++) {
            for (int z = 0; z < WORLD_SIZE_Z; z++) {
                VoxelType voxelType;
                int height = (int)(terrain[x * WORLD_SIZE_X + z] * WORLD_SIZE_Y);
                if (y < height) {
                    voxelType = y <= 2 ? VoxelType::STONE : VoxelType::GROUND;
                } else if (y == height) {
                    voxelType = VoxelType::GRASS;
                } else {
                    voxelType = VoxelType::NOTHING;
                }
                int chunkIndexX = x / Chunk::SIZE_X;
                int chunkIndexY = y / Chunk::SIZE_Y;
                int chunkIndexZ = z / Chunk::SIZE_Z;
                int voxelIndexX = x % Chunk::SIZE_X;
                int voxelIndexY = y % Chunk::SIZE_Y;
                int voxelIndexZ = z % Chunk::SIZE_Z;
                chunks
                    [chunkIndexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                     chunkIndexX * ChunksStorage::SIZE_X + chunkIndexZ]
                        .m_data
                            [voxelIndexY * Chunk::SIZE_X * Chunk::SIZE_Z +
                             voxelIndexX * Chunk::SIZE_X + voxelIndexZ]
                        .type = voxelType;
            }
        }
    }
}

void ChunksStorage::setVoxel(int x, int y, int z, VoxelType type) {
    if (x < 0 || y < 0 || z < 0 || x >= WORLD_SIZE_X || y >= WORLD_SIZE_Y || z >= WORLD_SIZE_Z)
        return;
    int chunkIndexX = x / Chunk::SIZE_X;
    int chunkIndexY = y / Chunk::SIZE_Y;
    int chunkIndexZ = z / Chunk::SIZE_Z;
    chunks
        [chunkIndexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
         chunkIndexX * ChunksStorage::SIZE_X + chunkIndexZ]
            .set(x % Chunk::SIZE_X, y % Chunk::SIZE_Y, z % Chunk::SIZE_Z, type);
}

Voxel *ChunksStorage::getVoxel(int x, int y, int z) {
    if (x < 0 || y < 0 || z < 0 || x >= WORLD_SIZE_X || y >= WORLD_SIZE_Y || z >= WORLD_SIZE_Z)
        return nullptr;
    int chunkIndexX = x / Chunk::SIZE_X;
    int chunkIndexY = y / Chunk::SIZE_Y;
    int chunkIndexZ = z / Chunk::SIZE_Z;
    return chunks
        [chunkIndexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
         chunkIndexX * ChunksStorage::SIZE_X + chunkIndexZ]
            .get(x % Chunk::SIZE_X, y % Chunk::SIZE_Y, z % Chunk::SIZE_Z);
}

Chunk *ChunksStorage::getChunk(int x, int y, int z) {
    if (x < 0 || y < 0 || z < 0 || x >= WORLD_SIZE_X || y >= WORLD_SIZE_Y || z >= WORLD_SIZE_Z)
        return nullptr;
    int chunkIndexX = x / Chunk::SIZE_X;
    int chunkIndexY = y / Chunk::SIZE_Y;
    int chunkIndexZ = z / Chunk::SIZE_Z;
    return &chunks
        [chunkIndexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
         chunkIndexX * ChunksStorage::SIZE_X + chunkIndexZ];
}

std::optional<VoxelRaycastData> ChunksStorage::bresenham3D(
    float x1, float y1, float z1, float x2, float y2, float z2, int maximumDistance
) {
    float t = 0.0f;
    int ix = (int)floor(x1);
    int iy = (int)floor(y1);
    int iz = (int)floor(z1);

    float stepx = (x2 > 0.0f) ? 1.0f : -1.0f;
    float stepy = (y2 > 0.0f) ? 1.0f : -1.0f;
    float stepz = (z2 > 0.0f) ? 1.0f : -1.0f;

    float infinity = std::numeric_limits<float>::infinity();

    float txDelta = (x2 == 0.0f) ? infinity : abs(1.0f / x2);
    float tyDelta = (y2 == 0.0f) ? infinity : abs(1.0f / y2);
    float tzDelta = (z2 == 0.0f) ? infinity : abs(1.0f / z2);

    float xdist = (stepx > 0) ? (ix + 1 - x1) : (x1 - ix);
    float ydist = (stepy > 0) ? (iy + 1 - y1) : (y1 - iy);
    float zdist = (stepz > 0) ? (iz + 1 - z1) : (z1 - iz);

    float txMax = (txDelta < infinity) ? txDelta * xdist : infinity;
    float tyMax = (tyDelta < infinity) ? tyDelta * ydist : infinity;
    float tzMax = (tzDelta < infinity) ? tzDelta * zdist : infinity;

    int steppedIndex = -1;

    glm::vec3 end;
    glm::vec3 normal;

    while (t <= maximumDistance) {
        Voxel *voxel = getVoxel(ix, iy, iz);
        if (voxel != nullptr && voxel->type != VoxelType::NOTHING) {
            end.x = ix;
            end.y = iy;
            end.z = iz;

            normal.x = 0;
            normal.y = 0;
            normal.z = 0;
            if (steppedIndex == 0) normal.x = (int)-stepx;
            if (steppedIndex == 1) normal.y = (int)-stepy;
            if (steppedIndex == 2) normal.z = (int)-stepz;
            return VoxelRaycastData(voxel, end, normal);
        }
        if (txMax < tyMax) {
            if (txMax < tzMax) {
                ix += stepx;
                t = txMax;
                txMax += txDelta;
                steppedIndex = 0;
            } else {
                iz += stepz;
                t = tzMax;
                tzMax += tzDelta;
                steppedIndex = 2;
            }
        } else {
            if (tyMax < tzMax) {
                iy += stepy;
                t = tyMax;
                tyMax += tyDelta;
                steppedIndex = 1;
            } else {
                iz += stepz;
                t = tzMax;
                tzMax += tzDelta;
                steppedIndex = 2;
            }
        }
    }
    return {};
}
