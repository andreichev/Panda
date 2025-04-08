//
//  Application.cpp
//  Main
//
//  Created by Admin on 22.07.2021.
//

#include "TriangleLayer.hpp"

#include <Foundation/PlatformDetection.hpp>
#include <Fern/Fern.hpp>
#include <Panda.hpp>

int startApp(int argc, char **argv) {
    Fern::initialize();
    Fern::Rect rect = Fern::Rect(0, 0, 900, 600);
    Fern::Window *window = Fern::createWindow(
        "Hello triangle",
        rect,
        Fern::WindowState::WindowStateNormal,
        Fern::DrawingContextType::DrawingContextTypeOpenGL
    );
    Miren::initialize(window->getDrawingContext());
    auto application = new Panda::Application();
    Panda::Application::get()->setMainWindow(window);
    // application->createImGuiLayer();
    application->pushLayer(new TriangleLayer);
    application->loop();
    delete application;
    Miren::terminate();
    Fern::disposeWindow(window);
    return 0;
}
