//
// Created by Admin on 03/05/2022.
//

#include "FullScreenToggle.hpp"

FullScreenToggle::FullScreenToggle() {
    m_window = Panda::Application::get()->getMainWindow();
}

void FullScreenToggle::update(double deltaTime) {
    if (Panda::Input::isKeyJustPressed(Fern::Key::ESCAPE)) { Panda::Application::get()->close(); }
    if (Panda::Input::isKeyJustPressed(Fern::Key::F)) {
        m_window->setState(
            m_window->getState() == Fern::WindowState::WindowStateFullScreen
                ? Fern::WindowState::WindowStateNormal
                : Fern::WindowState::WindowStateFullScreen
        );
    }
}
