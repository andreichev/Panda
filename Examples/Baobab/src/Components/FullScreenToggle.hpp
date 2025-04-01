//
// Created by Admin on 03/05/2022.
//

#pragma once

#include <Panda.hpp>

class FullScreenToggle final {
public:
    FullScreenToggle();
    void update(double deltaTime);

private:
    Fern::Window *m_window;
};
