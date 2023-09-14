//
// Created by Admin on 22.01.2022.
//

#pragma once

#include "Panda/Events/EventQueue.hpp"
#include "Panda/Base/Base.hpp"
#include "Panda/Window/Cursor.hpp"

namespace Panda {

class Window {
public:
    virtual ~Window() = default;
    virtual bool isFullScreen() = 0;
    virtual void setFullScreen(bool isFullScreen) = 0;
    virtual bool isMaximized() = 0;
    virtual void setMaximized(bool isMaximized) = 0;
    virtual void pollEvents() = 0;
    virtual void toggleCursorLock() = 0;
    virtual bool isCursorLocked() = 0;
    virtual void setCursor(Cursor cursor) = 0;
    virtual void setEventQueue(EventQueue *eventQueue) = 0;
    virtual Size getSize() = 0;
    virtual Size getDpi() = 0;
};

} // namespace Panda
