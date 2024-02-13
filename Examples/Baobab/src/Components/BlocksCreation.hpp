//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "CameraMove.hpp"
#include "Model/ChunksStorage.hpp"

#include <Panda.hpp>

class BlocksCreation : public Panda::NativeScript {
public:
    const int MAXIMUM_DISTANCE = 100;

    void initialize() override;
    void update(double deltaTime) override;
    void onImGuiRender() override;
    void setChunksStorage(Foundation::Shared<ChunksStorage> storage);
    void setCameraMove(CameraMove *cameraMove);
    void setLayoutHandle(Miren::VertexLayoutHandle layoutHandle);

private:
    void setVoxel(int x, int y, int z, VoxelType type);
    void updateChunk(int chunkIndexX, int chunkIndexY, int chunkIndexZ);

    Foundation::Shared<ChunksStorage> m_chunksStorage;
    Panda::TransformComponent *m_transform;
    CameraMove *m_cameraMove;
    Miren::VertexLayoutHandle m_layoutHandle;
    VoxelType m_selectedBlock;
};
