//
// Created by Admin on 22.01.2022.
//

#pragma once

#include <Foundation/Foundation.hpp>

#include "Panda/Base/Base.hpp"

namespace Panda {

struct ApplicationStartupSettings {
public:
    const char *name;
    const char *windowTitle;
    Size windowSize;
    bool isFullScreen;
    bool isMaximized;

    ApplicationStartupSettings()
        : name("Panda Sandbox App")
        , windowTitle("Panda")
        , windowSize(Size(600, 400))
        , isFullScreen(false)
        , isMaximized(false) {}

    ApplicationStartupSettings(
        const char *name,
        const char *windowTitle,
        const Size &windowSize,
        bool isFullScreen,
        bool isMaximized
    )
        : name(name)
        , windowTitle(windowTitle)
        , windowSize(windowSize)
        , isFullScreen(isFullScreen)
        , isMaximized(isMaximized) {}
};

} // namespace Panda