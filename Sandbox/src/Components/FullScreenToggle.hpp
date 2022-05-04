//
// Created by Admin on 03/05/2022.
//

#pragma once

#include "Panda.hpp"

class FullScreenToggle : public Panda::Component {
    void initialize() override;
    void update(float deltaTime) override;

    Panda::Input *m_events;
    Panda::Window *m_window;
};
