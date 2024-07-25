//
//  Application.cpp
//  Main
//
//  Created by Admin on 22.07.2021.
//

#include "pch.hpp"
#include "Levels/BaseLevel.hpp"

int startApp(int argc, char **argv) {
    Panda::ApplicationStartupSettings settings;
    settings.name = "HelloPbr";
    settings.windowTitle = "Hello Pbr";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    settings.isMaximized = false;
    auto application = new Panda::Application(settings);
    application->startBasicGame(new BaseLevel);
    application->loop();
    delete application;
    return 0;
}
