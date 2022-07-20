//
// Created by Admin on 06/04/2022.
//

#pragma once

#include <Panda.hpp>

class CubeComponent;

class CubeLevel : public Panda::Level {
public:
    void start(Panda::World *world) override;
};
