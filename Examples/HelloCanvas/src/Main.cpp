//
//  Application.cpp
//  Main
//
//  Created by Admin on 22.07.2021.
//

#include "ExampleLayer.hpp"
#include "GameAssetHandler.hpp"

#include <Foundation/PlatformDetection.hpp>
#include <Fern/Fern.hpp>
#include <Panda.hpp>

int startApp(int argc, const char **argv) {
    Fern::initialize();
    Fern::Rect rect = Fern::Rect(0, 0, 900, 600);
    Fern::Window *window = Fern::createWindow(
        "Hello canvas",
        rect,
        Fern::WindowState::WindowStateNormal,
        Fern::DrawingContextType::DrawingContextTypeOpenGL
    );
    Miren::initialize(window->getDrawingContext());
    Panda::AssetHandler *assetHandler = new GameAssetHandler;
    Panda::GameContext::setAssetHandler(assetHandler);
    auto application = new Panda::Application();
    application->setMainWindow(window);
    application->createImGuiLayer();
    application->pushLayer(new ExampleLayer(window));
    application->loop();
    delete application;
    delete assetHandler;
    Miren::terminate();
    Fern::disposeWindow(window);
    return 0;
}
