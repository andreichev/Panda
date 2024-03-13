//
// Created by Admin on 22.01.2022.
//

#pragma once

#include <Foundation/Foundation.hpp>

#include "Panda/Base/Base.hpp"
#include "Panda/GameLogic/Level.hpp"
#include "Panda/Application/Layer.hpp"

namespace Panda {

struct ApplicationStartupSettings {
public:
    const char *name;
    const char *windowTitle;
    Size windowSize;
    bool isFullScreen;
    bool isMaximized;
    Level *startupLevel;
    Layer *startupLayer;

    ApplicationStartupSettings()
        : name("Panda Sandbox App")
        , windowTitle("Panda")
        , windowSize(Size(600, 400))
        , isFullScreen(false)
        , isMaximized(false)
        , startupLevel(nullptr)
        , startupLayer(nullptr) {}

    ApplicationStartupSettings(
        const char *name,
        const char *windowTitle,
        const Size &windowSize,
        bool isFullScreen,
        bool isMaximized,
        Level *startupLevel,
        Layer *startupLayer
    )
        : name(name)
        , windowTitle(windowTitle)
        , windowSize(windowSize)
        , isFullScreen(isFullScreen)
        , isMaximized(isMaximized)
        , startupLevel(startupLevel)
        , startupLayer(startupLayer) {}
};

} // namespace Panda