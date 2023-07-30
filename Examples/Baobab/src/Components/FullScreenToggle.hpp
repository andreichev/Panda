//
// Created by Admin on 03/05/2022.
//

#pragma once

#include <Panda.hpp>

class FullScreenToggle : public Panda::NativeScript {
    void initialize() override;
    void update(double deltaTime) override;

    Panda::Window *m_window;
};
