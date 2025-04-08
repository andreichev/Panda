
#pragma once

#include "Fern/Events/Event.hpp"

namespace Fern {

struct TouchBeganEvent : public Event {
    TouchBeganEvent(int id, float x, float y)
        : Event(EventType::TouchBegan)
        , id(id)
        , x(x)
        , y(y) {}

    int id;
    float x, y;
};

struct TouchMovedEvent : public Event {
    TouchMovedEvent(int id, float x, float y)
        : Event(EventType::TouchMoved)
        , id(id)
        , x(x)
        , y(y) {}

    int id;
    float x, y;
};

struct TouchEndedEvent : public Event {
    TouchEndedEvent(int id)
        : Event(EventType::TouchEnded)
        , id(id) {}

    int id;
};

} // namespace Fern
