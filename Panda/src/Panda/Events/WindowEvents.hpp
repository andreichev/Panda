//
// Created by Admin on 19.02.2022.
//

#include "Event.hpp"

namespace Panda {

class WindowResizeEvent : public Event {
public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : Event(EventType::WindowResize),
        width(width) , height(height) {}

    unsigned int getWidth() const {
        return width;
    }

    unsigned int getHeight() const {
        return height;
    }

private:
    unsigned int width, height;
};

}