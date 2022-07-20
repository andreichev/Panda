//
// Created by Admin on 19.02.2022.
//

#include "pndpch.hpp"
#include "EventQueue.hpp"
#include "KeyEvents.hpp"
#include "MouseEvents.hpp"
#include "WindowEvents.hpp"

namespace Panda {

EventQueue::EventQueue()
    : events() {}

EventQueue::~EventQueue() {
    for (const Event *event = poll(); event != nullptr; event = poll()) {
        release(event);
    }
}

void EventQueue::release(const Event *event) {
    delete event;
}

Event *EventQueue::poll() {
    if (events.empty()) {
        return nullptr;
    }
    Event *event = events.front();
    events.pop();
    return event;
}

void EventQueue::postMouseButtonEvent(MouseButton button, bool pressed) {
    post(new MouseKeyEvent(button, pressed));
}

void EventQueue::postSizeEvent(unsigned int width, unsigned int height) {
    post(new WindowResizeEvent(width, height));
}

void EventQueue::postKeyEvent(Key key, bool down) {
    Event *ev;
    if (down) {
        ev = new KeyPressedEvent(key);
    } else {
        ev = new KeyReleasedEvent(key);
    }
    post(ev);
}

void EventQueue::postMouseEvent(int x, int y) {
    post(new MouseMovedEvent(x, y));
}

void EventQueue::postWindowCloseEvent() {
    post(new WindowCloseEvent());
}

void EventQueue::post(Event *event) {
    events.push(event);
}

} // namespace Panda
