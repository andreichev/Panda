//
// Created by Admin on 22.01.2022.
//

#pragma once

#include <Foundation/Foundation.hpp>

#include "Panda/Base/Base.hpp"
#include "Panda/GameLogic/Level.hpp"

namespace Panda {

struct ApplicationStartupSettings {
public:
    const char *name;
    const char *windowTitle;
    Size windowSize;
    bool isFullScreen;
    Level *startupLevel;

    ApplicationStartupSettings()
        : name("Panda Sandbox App")
        , windowTitle("Panda")
        , windowSize(Size(600, 400))
        , isFullScreen(false)
        , startupLevel(nullptr) {}

    ApplicationStartupSettings(const char *name,
        const char *windowTitle,
        const Size &windowSize,
        bool isFullScreen,
        Level *startupLevel)
        : name(name)
        , windowTitle(windowTitle)
        , windowSize(windowSize)
        , isFullScreen(isFullScreen)
        , startupLevel(startupLevel) {}
};

} // namespace Panda