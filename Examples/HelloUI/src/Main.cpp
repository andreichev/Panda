//
//  Application.cpp
//  Main
//
//  Created by Admin on 22.07.2021.
//

#include "ExampleLayer.hpp"

#include <Foundation/PlatformDetection.hpp>
#include <Fern/Fern.hpp>
#include <Panda.hpp>

int startApp(int argc, const char **argv) {
    Fern::initialize();
    Fern::Rect rect = Fern::Rect(0, 0, 900, 600);
    Fern::Window *window = Fern::createWindow(
        "Panda UI Sandbox",
        rect,
        Fern::WindowState::WindowStateNormal,
        Fern::DrawingContextType::DrawingContextTypeOpenGL
    );
    Miren::initialize(window->getDrawingContext());
    auto application = new Panda::Application();
    application->setMainWindow(window);
    // application->createImGuiLayer();
    application->pushLayer(new ExampleLayer());
    application->loop();
    delete application;
    Miren::terminate();
    Fern::disposeWindow(window);
    return 0;
}
