//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Model/ChunksStorage.hpp"

#include <Panda.hpp>

class BlocksCreation : public Panda::Component {
public:
    void initialize() override;
    void update(float deltaTime) override;

private:
    ChunksStorage *chunksStorage;
};
