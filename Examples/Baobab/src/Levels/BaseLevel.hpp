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
    Foundation::Shared<ChunksStorage> chunksStorage;
    Miren::ProgramHandle baseShader;
    Miren::TextureHandle texture;
};
