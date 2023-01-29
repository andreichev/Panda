//
// Created by Admin on 12.02.2022.
//

#include "BlocksCreation.hpp"
#include "Model/VoxelMeshGenerator.hpp"

void BlocksCreation::initialize() {
    transform = getEntity().getTransform();
}

void BlocksCreation::updateChunk(int chunkIndexX, int chunkIndexY, int chunkIndexZ) {
    Panda::MeshData primitiveMeshData = VoxelMeshGenerator::makeOneChunkMesh(*chunksStorage, chunkIndexX, chunkIndexY, chunkIndexZ, true);
    chunksStorage->chunks[chunkIndexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z + chunkIndexX * ChunksStorage::SIZE_X + chunkIndexZ]
        .getMesh()
        ->updateBuffer(primitiveMeshData);
}

void BlocksCreation::setVoxel(int x, int y, int z, int8_t id) {
    if (x < 0 || y < 0 || z < 0 || x >= ChunksStorage::WORLD_SIZE_X || y >= ChunksStorage::WORLD_SIZE_Y || z >= ChunksStorage::WORLD_SIZE_Z)
        return;

    chunksStorage->setVoxel(x, y, z, id);
    int chunkIndexX = x / Chunk::SIZE_X;
    int chunkIndexY = y / Chunk::SIZE_Y;
    int chunkIndexZ = z / Chunk::SIZE_Z;
    updateChunk(chunkIndexX, chunkIndexY, chunkIndexZ);

    if (x % Chunk::SIZE_X == 0 && chunkIndexX > 0) {
        updateChunk(chunkIndexX - 1, chunkIndexY, chunkIndexZ);
    }
    if (x % Chunk::SIZE_X == Chunk::SIZE_X - 1 && chunkIndexX < ChunksStorage::SIZE_X - 1) {
        updateChunk(chunkIndexX + 1, chunkIndexY, chunkIndexZ);
    }
    if (z % Chunk::SIZE_Z == 0 && chunkIndexZ > 0) {
        updateChunk(chunkIndexX, chunkIndexY, chunkIndexZ - 1);
    }
    if (z % Chunk::SIZE_Z == Chunk::SIZE_Z - 1 && chunkIndexZ < ChunksStorage::SIZE_Z - 1) {
        updateChunk(chunkIndexX, chunkIndexY, chunkIndexZ + 1);
    }
    if (y % Chunk::SIZE_Y == 0 && chunkIndexY > 0) {
        updateChunk(chunkIndexX, chunkIndexY - 1, chunkIndexZ);
    }
    if (y % Chunk::SIZE_Y == Chunk::SIZE_Y - 1 && chunkIndexY < ChunksStorage::SIZE_Y - 1) {
        updateChunk(chunkIndexX, chunkIndexY + 1, chunkIndexZ);
    }
}

void BlocksCreation::update(double deltaTime) {
    glm::vec4 position = transform->getPosition();
    glm::vec4 target = transform->getFront();
    VoxelRaycastData *v = chunksStorage->bresenham3D(position.x, position.y, position.z, target.x, target.y, target.z, maximumDistance);

    if (v != nullptr && v->voxel != nullptr) {
        if (Panda::Input::isMouseButtonJustPressed(Panda::MouseButton::LEFT)) {
            int x = v->end.x + v->normal.x;
            int y = v->end.y + v->normal.y;
            int z = v->end.z + v->normal.z;
            setVoxel(x, y, z, 11);
        } else if (Panda::Input::isMouseButtonJustPressed(Panda::MouseButton::RIGHT)) {
            int x = v->end.x;
            int y = v->end.y;
            int z = v->end.z;
            setVoxel(x, y, z, 0);
        }
    }
}

void BlocksCreation::setChunksStorage(Foundation::Shared<ChunksStorage> storage) {
    this->chunksStorage = storage;
}
