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
    settings.name = "HelloTerrain";
    settings.windowTitle = "Hello Terrain";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    settings.isMaximized = false;
    settings.startupLevel = new BaseLevel();
    runPandaApplication(settings);
    return 0;
}
