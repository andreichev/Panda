//
//  Application.cpp
//  Main
//
//  Created by Admin on 22.07.2021.
//

#include "ExampleLevel.hpp"

int startApp(int argc, char **argv) {
    Panda::ApplicationStartupSettings settings;
    settings.name = "Panda UI Sandbox";
    settings.windowTitle = "Panda UI Sandbox";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    settings.startupLevel = new ExampleLevel();
    runPandaApplication(settings);
    return 0;
}
