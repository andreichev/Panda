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
    settings.windowSize = {844, 390};
    settings.isFullScreen = false;
    settings.startupLevel = new ExampleLevel();
    runPandaApplication(settings);
    return 0;
}
