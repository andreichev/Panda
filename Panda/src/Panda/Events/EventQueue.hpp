//
// Created by Admin on 19.02.2022.
//

#pragma once

#include "Event.hpp"
#include "Key.hpp"

namespace Panda {

class EventQueue {
public:
    EventQueue();
    ~EventQueue();
    void release(const Event *event);
    void postMouseButtonEvent(MouseButton button, bool pressed);
    void postSizeEvent(unsigned int width, unsigned int height);
    void postKeyEvent(Key key, bool down);
    void postMouseEvent(int x, int y);
    void postWindowCloseEvent();
    Event *poll();

private:
    void post(Event *event);
    std::queue<Event *> events;
};

} // namespace Panda