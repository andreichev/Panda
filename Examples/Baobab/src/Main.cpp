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
    settings.name = "Baobab";
    settings.windowTitle = "BAOBAB Craft";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    settings.startupLevel = new BaseLevel();
    runPandaApplication(settings);
    return 0;
}