//
//  Application.cpp
//  Main
//
//  Created by Admin on 22.07.2021.
//

#include "TriangleLevel.hpp"

int startApp(int argc, char **argv) {
    Panda::ApplicationStartupSettings settings;
    settings.name = "Hello triangle";
    settings.windowTitle = "Hello triangle";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    settings.startupLevel = new TriangleLevel();
    runPandaApplication(settings);
    return 0;
}
