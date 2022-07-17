//
// Created by Admin on 03/05/2022.
//

#include "FullScreenToggle.hpp"

void FullScreenToggle::initialize() {
    m_events = &Panda::ApplicationContext::get()->getInput();
    m_window = &Panda::ApplicationContext::get()->getWindow();
}

void FullScreenToggle::update(double deltaTime) {
    if (m_events->isKeyJustPressed(Panda::Key::ESCAPE)) {
        Panda::ApplicationContext::get()->isApplicationShouldClose = true;
    }
    if (m_events->isKeyJustPressed(Panda::Key::F)) {
        m_window->setFullScreen(m_window->isFullScreen() == false);
    }
}
