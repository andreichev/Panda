//
//  Application.cpp
//  Main
//

#include "EditorLayer/EditorLayer.hpp"
#include "ProjectLoader/AssetHandlerEditor.hpp"

#include <Foundation/PlatformDetection.hpp>
#include <Fern/Fern.hpp>
#include <Panda.hpp>

int startApp(int argc, const char **argv) {
    Fern::initialize();
    Fern::Rect rect = Fern::Rect(0, 0, 1400, 800);
    Fern::Window *window = Fern::createWindow(
        "Panda Editor",
        rect,
        Fern::WindowState::WindowStateNormal,
        Fern::DrawingContextType::DrawingContextTypeOpenGL
    );
    Miren::initialize(window->getDrawingContext());
    Panda::AssetHandler *assetHandler = new Panda::AssetHandlerEditor;
    Panda::GameContext::setAssetHandler(assetHandler);
    auto application = new Panda::Application();
    application->setMainWindow(window);
    application->createImGuiLayer();
    application->pushLayer(new Panda::EditorLayer(window));
    application->loop();
    delete application;
    delete assetHandler;
    Miren::terminate();
    Fern::disposeWindow(window);
    return 0;
}
