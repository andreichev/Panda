//
// Created by Admin on 19.02.2022.
//

#include "Panda/Events/EventQueue.hpp"
#include "Panda/Events/KeyEvents.hpp"
#include "Panda/Events/MouseEvents.hpp"
#include "Panda/Events/TouchEvents.hpp"
#include "Panda/Events/WindowEvents.hpp"

namespace Panda {

EventQueue::EventQueue()
    : m_events(30000) {}

void EventQueue::finishWriting() {
    m_events.finishWriting();
}

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

void EventQueue::postMouseEvent(double x, double y) {
    MouseMovedEvent event(x, y);
    m_events.write(event);
}

void EventQueue::postScrollEvent(double xoffset, double yoffset, bool isTrackpad) {
    MouseScrolledEvent event(xoffset, yoffset, isTrackpad);
    m_events.write(event);
}

void EventQueue::postWindowCloseEvent() {
    WindowCloseEvent event;
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

} // namespace Panda
