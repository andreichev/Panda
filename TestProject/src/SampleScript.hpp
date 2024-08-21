//
// Created by Michael Andreichev on 03.08.2024.
//

#pragma once

#include <ScriptCore.hpp>
#include <ScriptRegistry.hpp>

#include <iostream>

class SampleScript : public Panda::Script {
public:
    void update(float dt) override {
        std::cout << "HELLO, WORLD!" << std::endl;
    }
};

REGISTER_SCRIPT(SampleScript)
