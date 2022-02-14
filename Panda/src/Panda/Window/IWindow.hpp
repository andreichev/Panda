//
// Created by Admin on 22.01.2022.
//

#pragma once

#include "Panda/Base/GSize.hpp"

namespace Panda {

class IEvents;

class IWindow {
public:
    virtual ~IWindow() = default;
    virtual void initialize(const char *title, GSize size, bool isFullscreen) = 0;
    virtual double getTime() = 0;
    virtual bool isShouldClose() = 0;
    virtual void setShouldClose(bool flag) = 0;
    virtual bool isFullScreen() = 0;
    virtual void setFullScreen(bool isFullScreen) = 0;
    virtual GSize getWindowSize() = 0;
    virtual void swapBuffers() = 0;
    virtual void *getNativeWindow() = 0;
    virtual void *getProcAddressFuncPointer() = 0;
};

} // namespace Panda