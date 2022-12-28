//
// Created by Admin on 19.02.2022.
//

#pragma once

#include "Event.hpp"

namespace Panda {

struct MouseMovedEvent : public Event {
    MouseMovedEvent(int x, int y)
        : Event(EventType::MouseMoved)
        , x(x)
        , y(y) {}

    int x, y;
};

struct MouseKeyEvent : public Event {
    MouseKeyEvent(MouseButton button, bool isPressed)
        : Event(isPressed ? EventType::MouseButtonPressed : EventType::MouseButtonReleased)
        , button(button) {}

    MouseButton button;
};

} // namespace Panda
