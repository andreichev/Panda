//
//  Application.cpp
//  Main
//
//  Created by Admin on 22.07.2021.
//

#include "CubeLevel.hpp"

int startApp(int argc, char **argv) {
    Panda::ApplicationStartupSettings settings;
    settings.name = "Hello Cube";
    settings.windowTitle = "Hello Cube";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    auto application = new Panda::Application(settings);
    application->startBasicGame(new CubeLevel);
    application->loop();
    delete application;
    return 0;
}
