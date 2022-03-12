//
// Created by Admin on 19.02.2022.
//

#pragma once

#include "Event.hpp"

namespace Panda {

class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(int x, int y)
        : Event(EventType::MouseMoved)
        , x(x)
        , y(y) {}

    int x, y;
};

} // namespace Panda
