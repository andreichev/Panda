//
//  Application.cpp
//  Sandbox
//
//  Created by Admin on 22.07.2021.
//

#include "sndbxpch.hpp"
#include "Levels/BaseLevel.hpp"
#include "Levels/CubeLevel.hpp"
#include "Levels/TriangleLevel.hpp"

int startApp(int argc, char **argv) {
    Panda::ApplicationStartupSettings settings;
    settings.name = "ITIS Craft";
    settings.windowTitle = "ITIS Craft";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    settings.startupLevel = new BaseLevel();
    runPandaApplication(settings);
    return 0;
}
