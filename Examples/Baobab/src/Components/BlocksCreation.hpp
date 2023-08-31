//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Model/ChunksStorage.hpp"

#include <Panda.hpp>

class BlocksCreation : public Panda::NativeScript {
public:
    const int MAXIMUM_DISTANCE = 100;

    void initialize() override;
    void update(double deltaTime) override;
    void setChunksStorage(Foundation::Shared<ChunksStorage> storage);
    void setCamera(Panda::Camera *camera);

private:
    void setVoxel(int x, int y, int z, int8_t id);
    void updateChunk(int chunkIndexX, int chunkIndexY, int chunkIndexZ);

    Foundation::Shared<ChunksStorage> m_chunksStorage;
    Panda::Transform *m_transform;
    Panda::Camera *m_camera;
    int m_frame;
};
