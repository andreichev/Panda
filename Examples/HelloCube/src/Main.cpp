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
    settings.startupLevel = new CubeLevel();
    runPandaApplication(settings);
    return 0;
}
