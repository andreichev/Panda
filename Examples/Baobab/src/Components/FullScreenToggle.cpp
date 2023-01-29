//
// Created by Admin on 03/05/2022.
//

#include "FullScreenToggle.hpp"

void FullScreenToggle::initialize() {
    m_window = Panda::Application::get()->getWindow();
}

void FullScreenToggle::update(double deltaTime) {
    if (Panda::Input::isKeyJustPressed(Panda::Key::ESCAPE)) {
        Panda::Application::get()->close();
    }
    if (Panda::Input::isKeyJustPressed(Panda::Key::F)) {
        m_window->setFullScreen(m_window->isFullScreen() == false);
    }
}
