//
//  Application.cpp
//  Main
//

#include "EditorLayer/EditorLayer.hpp"

#include <Foundation/PlatformDetection.hpp>
#include <Fern/Fern.hpp>
#include <Panda.hpp>

int startApp(int argc, char **argv) {
    Fern::initialize();
    Fern::Rect rect = Fern::Rect(0, 0, 600, 400);
    Fern::Window *window = Fern::createWindow(
        "Panda Editor",
        rect,
        Fern::WindowState::WindowStateNormal,
        Fern::DrawingContextType::DrawingContextTypeOpenGL
    );
    Miren::initialize(window->getDrawingContext());
    auto application = new Panda::Application();
    Panda::Application::get()->setMainWindow(window);
    application->createImGuiLayer();
    application->pushLayer(new Panda::EditorLayer(window));
    application->loop();
    delete application;
    Miren::terminate();
    Fern::disposeWindow(window);
    return 0;
}
