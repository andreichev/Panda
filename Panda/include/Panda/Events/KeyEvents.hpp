//
// Created by Admin on 19.02.2022.
//

#include "Panda/Events/Event.hpp"
#include "Panda/Events/Key.hpp"

namespace Panda {

struct KeyPressedEvent : public Event {
    KeyPressedEvent(Key key)
        : Event(EventType::KeyPressed)
        , key(key) {}

    Key key;
};

struct KeyReleasedEvent : public Event {
    KeyReleasedEvent(Key key)
        : Event(EventType::KeyReleased)
        , key(key) {}

    Key key;
};

} // namespace Panda