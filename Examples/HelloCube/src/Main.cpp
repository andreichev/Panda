//
//  Application.cpp
//  Main
//
//  Created by Admin on 22.07.2021.
//

#include "CubeLayer.hpp"

#include <Foundation/PlatformDetection.hpp>
#include <Fern/Fern.hpp>
#include <Panda.hpp>

int startApp(int argc, char **argv) {
    Fern::initialize();
    auto application = new Panda::Application();
    Fern::Rect rect = Fern::Rect(0, 0, 900, 600);
    Fern::Window *window = Fern::createWindow(
        "Hello triangle",
        rect,
        Fern::WindowState::WindowStateNormal,
        Fern::DrawingContextType::DrawingContextTypeOpenGL
    );
    Panda::Application::get()->setMainWindow(window);
#ifdef PLATFORM_DESKTOP
    Miren::initialize(window->getDrawingContext());
#endif
    application->createImGuiLayer();
    application->pushLayer(new CubeLayer(window));
    application->loop();
    Fern::disposeWindow(window);
    delete application;
#ifdef PLATFORM_DESKTOP
    Miren::terminate();
#endif
    return 0;
}
