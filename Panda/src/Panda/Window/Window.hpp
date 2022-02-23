//
// Created by Admin on 22.01.2022.
//

#pragma once

#include "Panda/Base/GSize.hpp"

namespace Panda {

class Input;

class Window {
public:
    virtual ~Window() = default;
    virtual void initialize(const char *title, GSize size, bool isFullscreen) = 0;
    virtual bool isFullScreen() = 0;
    virtual void setFullScreen(bool isFullScreen) = 0;
    virtual GSize getWindowSize() = 0;
    virtual void pollEvents() = 0;
};

} // namespace Panda