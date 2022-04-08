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
    runPandaApplication(Panda::ApplicationStartupSettings::builder()
                            .name("ITIS Craft")
                            .windowTitle("ITIS Craft")
                            .windowSize({900, 600})
                            .isFullScreen(true)
                            .startupLevel(new BaseLevel())
                            .build());
    return 0;
}
