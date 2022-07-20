//
// Created by Admin on 22.01.2022.
//

#pragma once

#include "Panda/Base/GSize.hpp"
#include "Panda/Events/EventQueue.hpp"

namespace Panda {

class Window {
public:
    virtual ~Window() = default;
    virtual bool isFullScreen() = 0;
    virtual void setFullScreen(bool isFullScreen) = 0;
    virtual void pollEvents() = 0;
    virtual void toggleCursorLock() = 0;
    virtual bool isCursorLocked() = 0;
    virtual void setEventQueue(EventQueue *eventQueue) = 0;
    virtual GSize getSize() = 0;
};

} // namespace Panda
