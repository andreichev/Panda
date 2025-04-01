//
// Created by Admin on 19.02.2022.
//

#pragma once

#include "Fern/Events/Event.hpp"
#include "Fern/Events/Key.hpp"

#include <Foundation/CommandBuffer.hpp>

namespace Fern {

class Window;

class EventQueue {
public:
    EventQueue();
    void finishWriting();
    void reset();
    void postMouseButtonEvent(MouseButton button, bool pressed);
    void postSizeEvent(uint32_t width, uint32_t height);
    void postKeyEvent(Key key, bool down);
    void postCharEvent(uint32_t c);
    void postMouseEvent(double x, double y, double dx, double dy);
    void postTouchBeganEvent(int id, float x, float y);
    void postTouchMovedEvent(int id, float x, float y);
    void postTouchEndedEvent(int id);
    void postScrollEvent(double xoffset, double yoffset, bool isTrackpad);
    void postWindowCloseRequest(Window *window);
    void postAppQuitRequest();
    Event *poll();

private:
    Foundation::CommandBuffer m_events;
};

} // namespace Fern
