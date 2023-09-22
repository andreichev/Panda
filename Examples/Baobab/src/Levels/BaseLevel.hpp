//
// Created by Admin on 07.02.2022.
//

#pragma once

#include "Panda.hpp"
#include "Model/ChunksStorage.hpp"

class BaseLevel : public Panda::Level {
public:
    void start(Panda::World *world) override;
    ~BaseLevel();

private:
    Foundation::Shared<ChunksStorage> m_chunksStorage;
    Miren::ProgramHandle m_groundShader;
    Miren::TextureHandle m_blocksTileTexture;
};
