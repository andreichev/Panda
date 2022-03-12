//
// Created by Admin on 19.02.2022.
//

#pragma once

namespace Panda {

enum class EventType {
    None = 0,
    WindowResize,
    // WindowClose,
    // WindowFocus,
    // WindowLostFocus,
    KeyPressed,
    KeyReleased,
    // MouseButtonPressed,
    // MouseButtonReleased,
    MouseMoved,
    // MouseScrolled
};

class Event {
public:
    virtual ~Event() = default;
    Event(EventType type)
        : type(type) {}

    EventType type;
    bool isHandled = false;
};

} // namespace Panda
