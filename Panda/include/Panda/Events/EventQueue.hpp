//
// Created by Admin on 19.02.2022.
//

#pragma once

#include "Panda/Events/Event.hpp"
#include "Panda/Events/Key.hpp"

#include <Foundation/CommandBuffer.hpp>

namespace Panda {

class EventQueue {
public:
    EventQueue();
    void finishWriting();
    void reset();
    void postMouseButtonEvent(MouseButton button, bool pressed);
    void postSizeEvent(uint32_t width, uint32_t height);
    void postKeyEvent(Key key, bool down);
    void postCharEvent(uint32_t c);
    void postMouseEvent(double x, double y);
    void postTouchBeganEvent(int id, float x, float y);
    void postTouchMovedEvent(int id, float x, float y);
    void postTouchEndedEvent(int id);
    void postScrollEvent(double xoffset, double yoffset, bool isTrackpad);
    void postWindowCloseEvent();
    Event *poll();

private:
    Foundation::CommandBuffer m_events;
};

} // namespace Panda
