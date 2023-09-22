//
// Created by Admin on 12.02.2022.
//

#include "BlocksCreation.hpp"
#include "Model/VoxelMeshGenerator.hpp"

#include <imgui.h>

void BlocksCreation::initialize() {
    m_transform = &getEntity().getTransform();
    m_selectedBlock = VoxelType::GROUND;
}

void BlocksCreation::updateChunk(int chunkIndexX, int chunkIndexY, int chunkIndexZ) {
    // LOG_INFO("UPDATE CHUNK {} {} {}", chunkIndexX, chunkIndexY, chunkIndexZ);
    Panda::MeshData data = VoxelMeshGenerator::makeOneChunkMesh(
        m_layoutHandle, *m_chunksStorage, chunkIndexX, chunkIndexY, chunkIndexZ, true);
    m_chunksStorage
        ->chunks[chunkIndexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                 chunkIndexX * ChunksStorage::SIZE_X + chunkIndexZ]
        .getMesh()
        ->update(data);
}

void BlocksCreation::setVoxel(int x, int y, int z, VoxelType type) {
    if (x < 0 || y < 0 || z < 0 || x >= ChunksStorage::WORLD_SIZE_X ||
        y >= ChunksStorage::WORLD_SIZE_Y || z >= ChunksStorage::WORLD_SIZE_Z)
        return;

    m_chunksStorage->setVoxel(x, y, z, type);
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
    bool leftPressed;
    bool rightPressed;
    if (Panda::Input::isKeyPressed(Panda::Key::E)) {
        leftPressed = Panda::Input::isMouseButtonPressed(Panda::MouseButton::LEFT);
        rightPressed = Panda::Input::isMouseButtonPressed(Panda::MouseButton::RIGHT);
    } else {
        leftPressed = Panda::Input::isMouseButtonJustPressed(Panda::MouseButton::LEFT);
        rightPressed = Panda::Input::isMouseButtonJustPressed(Panda::MouseButton::RIGHT);
    }
    if (!leftPressed && !rightPressed) {
        return;
    }
    glm::vec4 position = m_transform->getPosition();
    glm::vec3 target = m_camera->getFront();
    auto v = m_chunksStorage->bresenham3D(
        position.x, position.y, position.z, target.x, target.y, target.z, MAXIMUM_DISTANCE);
    if (v && v->voxel != nullptr) {
        if (leftPressed) {
            int x = v->end.x + v->normal.x;
            int y = v->end.y + v->normal.y;
            int z = v->end.z + v->normal.z;
            setVoxel(x, y, z, m_selectedBlock);
        } else if (rightPressed) {
            int x = v->end.x;
            int y = v->end.y;
            int z = v->end.z;
            setVoxel(x, y, z, VoxelType::NOTHING);
        }
    }
}

void BlocksCreation::onImGuiRender() {
    ImGui::Begin("Block");
    if (ImGui::Button("TREE")) {
        m_selectedBlock = VoxelType::TREE;
    }
    if (ImGui::Button("STONE BRICKS")) {
        m_selectedBlock = VoxelType::STONE_BRICKS;
    }
    if (ImGui::Button("BOARDS")) {
        m_selectedBlock = VoxelType::BOARDS;
    }
    ImGui::End();
}

void BlocksCreation::setChunksStorage(Foundation::Shared<ChunksStorage> storage) {
    m_chunksStorage = storage;
}

void BlocksCreation::setCamera(Panda::Camera *camera) {
    m_camera = camera;
}

void BlocksCreation::setLayoutHandle(Miren::VertexLayoutHandle layoutHandle) {
    m_layoutHandle = layoutHandle;
}
