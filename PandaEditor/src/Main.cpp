//
//  Application.cpp
//  Main
//

#include "EditorLayer/EditorLayer.hpp"

#include <Panda.hpp>

int startApp(int argc, char **argv) {
    Panda::ApplicationStartupSettings settings;
    settings.name = "Panda Editor";
    settings.windowTitle = "Panda Editor";
    settings.isFullScreen = false;
    auto application = new Panda::Application(settings);
    application->pushLayer(new Panda::EditorLayer);
    application->loop();
    delete application;
    return 0;
}
