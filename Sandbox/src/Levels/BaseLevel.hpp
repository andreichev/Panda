//
// Created by Admin on 07.02.2022.
//

#pragma once

#include "Panda.hpp"
#include "Model/ChunksStorage.hpp"

class BaseLevel : public Panda::Level {
public:
    void start(Panda::World *world) override;

private:
    Panda::Unique<ChunksStorage> chunksStorage;
    Panda::ShaderHandle baseShader;
    Panda::TextureHandle texture;
};
