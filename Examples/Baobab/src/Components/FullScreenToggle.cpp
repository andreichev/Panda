//
// Created by Admin on 03/05/2022.
//

#include "FullScreenToggle.hpp"

#include <Panda.hpp>

void FullScreenToggle::update(double deltaTime) {
    Fern::Window *window = Panda::Application::get()->getMainWindow();
    if (!window) { return; }
    if (Panda::Input::isKeyJustPressed(Fern::Key::ESCAPE)) { Panda::Application::get()->close(); }
    if (Panda::Input::isKeyJustPressed(Fern::Key::F)) {
        bool currentlyIsInFullScreen =
            window->getState() == Fern::WindowState::WindowStateFullScreen;
        window->setState(
            currentlyIsInFullScreen ? Fern::WindowState::WindowStateNormal
                                    : Fern::WindowState::WindowStateFullScreen
        );
        if (currentlyIsInFullScreen) { window->setSize({900, 600}); }
    }
}
