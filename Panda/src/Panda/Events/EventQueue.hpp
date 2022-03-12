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
    void post(Event *event);
    const Event *poll();

private:
    std::queue<Event *> events;
};

} // namespace Panda