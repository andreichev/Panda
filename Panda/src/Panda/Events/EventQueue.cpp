//
// Created by Admin on 19.02.2022.
//

#include "pndpch.hpp"
#include "EventQueue.hpp"
#include "KeyEvents.hpp"

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

const Event *EventQueue::poll() {
    if (events.empty()) {
        return nullptr;
    }
    Event *event = events.front();
    events.pop();
    return event;
}

void EventQueue::post(Event *event) {
    events.push(event);
}

} // namespace Panda
