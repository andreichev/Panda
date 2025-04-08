//
// Created by Admin on 19.02.2022.
//

#include "Fern/Events/EventQueue.hpp"
#include "Fern/Events/KeyEvents.hpp"
#include "Fern/Events/MouseEvents.hpp"
#include "Fern/Events/TouchEvents.hpp"
#include "Fern/Events/WindowEvents.hpp"

namespace Fern {

EventQueue::EventQueue()
    : m_events(30000) {}

void EventQueue::reset() {
    m_events.reset();
}

Event *EventQueue::poll() {
    return static_cast<Event *>(m_events.read());
}

void EventQueue::postMouseButtonEvent(MouseButton button, bool pressed) {
    MouseKeyEvent event(button, pressed);
    m_events.write(event);
}

void EventQueue::postSizeEvent(uint32_t width, uint32_t height) {
    WindowResizeEvent event(width, height);
    m_events.write(event);
}

void EventQueue::postKeyEvent(Key key, bool down) {
    if (down) {
        KeyPressedEvent event(key);
        m_events.write(event);
    } else {
        KeyReleasedEvent event(key);
        m_events.write(event);
    }
}

void EventQueue::postCharEvent(uint32_t c) {
    CharacterInputEvent event(c);
    m_events.write(event);
}

void EventQueue::postMouseEvent(double x, double y, double dx, double dy) {
    MouseMovedEvent event(x, y, dx, dy);
    m_events.write(event);
}

void EventQueue::postScrollEvent(double xoffset, double yoffset, bool isTrackpad) {
    MouseScrolledEvent event(xoffset, yoffset, isTrackpad);
    m_events.write(event);
}

void EventQueue::postWindowCloseRequest(Window *window) {
    WindowCloseRequestEvent event(window);
    m_events.write(event);
}

void EventQueue::postAppQuitRequest() {
    QuitRequestEvent event;
    m_events.write(event);
}

void EventQueue::postTouchBeganEvent(int id, float x, float y) {
    TouchBeganEvent event(id, x, y);
    m_events.write(event);
}

void EventQueue::postTouchMovedEvent(int id, float x, float y) {
    TouchMovedEvent event(id, x, y);
    m_events.write(event);
}

void EventQueue::postTouchEndedEvent(int id) {
    TouchEndedEvent event(id);
    m_events.write(event);
}

} // namespace Fern
