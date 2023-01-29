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
    void setChunksStorage(Foundation::Shared<ChunksStorage> storage);

private:
    void setVoxel(int x, int y, int z, int8_t id);
    void updateChunk(int chunkIndexX, int chunkIndexY, int chunkIndexZ);

    Foundation::Shared<ChunksStorage> chunksStorage;
    Foundation::Shared<Panda::Transform> transform;
    const int maximumDistance = 10;
};
