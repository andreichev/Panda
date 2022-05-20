//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Model/ChunksStorage.hpp"

#include <Panda.hpp>

class BlocksCreation : public Panda::Component {
public:
    void initialize() override;
    void update(double deltaTime) override;
    void setChunksStorage(Panda::Shared<ChunksStorage> storage);

private:
    void setVoxel(int x, int y, int z, int8_t id);
    void updateChunk(int chunkIndexX, int chunkIndexY, int chunkIndexZ);

    Panda::Shared<ChunksStorage> chunksStorage;
    Panda::Shared<Panda::Transform> transform;
    Panda::Input *events;
    const int maximumDistance = 10;
};
