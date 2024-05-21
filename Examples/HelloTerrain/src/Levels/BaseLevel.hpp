//
// Created by Admin on 07.02.2022.
//

#pragma once

#include "Panda.hpp"
#include "Model/TerrainMeshGenerator.hpp"

class BaseLevel : public Panda::Level {
public:
    void start(Panda::World *world) override;
    ~BaseLevel();

private:
    TerrainMeshGenerator m_meshGenerator;
    Miren::ProgramHandle m_shader;
    Miren::TextureHandle m_heightMapTexture;
};
