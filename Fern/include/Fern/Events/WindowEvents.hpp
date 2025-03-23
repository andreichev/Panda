//
// Created by Admin on 19.02.2022.
//

#include "Fern/Events/Event.hpp"

namespace Fern {

class WindowResizeEvent : public Event {
public:
    WindowResizeEvent(uint16_t width, uint16_t height)
        : Event(EventType::WindowResize)
        , width(width)
        , height(height) {}

    uint16_t getWidth() const {
        return width;
    }

    uint16_t getHeight() const {
        return height;
    }

private:
    uint16_t width, height;
};

class WindowCloseEvent : public Event {
public:
    WindowCloseEvent()
        : Event(EventType::WindowClose) {}
};

class QuitRequestEvent : public Event {
public:
    QuitRequestEvent()
        : Event(EventType::QuitRequest) {}
};

} // namespace Fern