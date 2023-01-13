//
// Created by Admin on 19.02.2022.
//

#pragma once

#include "Foundation/CommandBuffer.hpp"
#include "Event.hpp"
#include "Key.hpp"

namespace Panda {

class EventQueue {
public:
    EventQueue();
    void finishWriting();
    void reset();
    void postMouseButtonEvent(MouseButton button, bool pressed);
    void postSizeEvent(uint32_t width, uint32_t height);
    void postKeyEvent(Key key, bool down);
    void postMouseEvent(uint32_t x, uint32_t y);
    void postScrollEvent(double xoffset, double yoffset);
    void postWindowCloseEvent();
    Event *poll();

private:
    Foundation::CommandBuffer m_events;
};

} // namespace Panda