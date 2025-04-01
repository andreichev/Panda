//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "CameraMove.hpp"
#include "Model/ChunksStorage.hpp"

#include <Panda/GameLogic/Components/TransformComponent.hpp>
#include <Panda.hpp>

class BlocksCreation final {
public:
    const int MAXIMUM_DISTANCE = 100;

    BlocksCreation(Panda::TransformComponent *transform);
    void update(double deltaTime);
    void onImGuiRender();
    void setChunksStorage(ChunksStorage *storage);
    void setCameraMove(CameraMove *cameraMove);
    void setLayoutHandle(Miren::VertexLayoutHandle layoutHandle);

private:
    void setVoxel(int x, int y, int z, VoxelType type);
    void updateChunk(int chunkIndexX, int chunkIndexY, int chunkIndexZ);

    ChunksStorage *m_chunksStorage;
    Panda::TransformComponent *m_transform;
    CameraMove *m_cameraMove;
    Miren::VertexLayoutHandle m_layoutHandle;
    VoxelType m_selectedBlock;
};
