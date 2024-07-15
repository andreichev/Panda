//
//  Application.cpp
//  Main
//
//  Created by Admin on 22.07.2021.
//

#include "ExampleLevel.hpp"

int startApp(int argc, char **argv) {
    Panda::ApplicationStartupSettings settings;
    settings.name = "Hello canvas";
    settings.windowTitle = "Hello canvas";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    auto application = new Panda::Application(settings);
    application->startBasicGame(new ExampleLevel);
    application->loop();
    delete application;
    return 0;
}
