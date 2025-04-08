//
// Created by Admin on 19.02.2022.
//

#pragma once

#include "Fern/Events/Event.hpp"

namespace Fern {

struct MouseMovedEvent : public Event {
    MouseMovedEvent(double x, double y, double dx, double dy)
        : Event(EventType::MouseMoved)
        , x(x)
        , y(y)
        , dx(dx)
        , dy(dy) {}

    double x, y, dx, dy;
};

struct MouseScrolledEvent : public Event {
    MouseScrolledEvent(double xoffset, double yoffset, bool isTrackpad)
        : Event(EventType::MouseScrolled)
        , xoffset(xoffset)
        , yoffset(yoffset)
        , isTrackpad(isTrackpad) {}

    double xoffset;
    double yoffset;
    bool isTrackpad;
};

struct MouseKeyEvent : public Event {
    MouseKeyEvent(MouseButton button, bool isPressed)
        : Event(isPressed ? EventType::MouseButtonPressed : EventType::MouseButtonReleased)
        , button(button) {}

    MouseButton button;
};

} // namespace Fern
