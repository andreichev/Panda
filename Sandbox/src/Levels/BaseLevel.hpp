//
// Created by Admin on 07.02.2022.
//

#pragma once

#include <Panda/GameLogic/ILevel.hpp>
#include <Panda/Renderer/IShader.hpp>
#include <Panda/Renderer/ITexture.hpp>
#include "Model/ChunksStorage.hpp"

class BaseLevel : public Panda::ILevel {
public:
    void start(Panda::World *world) override;

private:
    Panda::Unique<ChunksStorage> chunksStorage;
    Panda::Shared<Panda::IShader> baseShader;
    Panda::Shared<Panda::ITexture> texture;
};
