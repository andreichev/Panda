//
// Created by Admin on 19.02.2022.
//

#include "Event.hpp"
#include "Key.hpp"

namespace Panda {

class KeyPressedEvent : public Event {
public:
    KeyPressedEvent(Key key)
        : Event(EventType::KeyPressed), key(key) {}

    Key key;
};

class KeyReleasedEvent : public Event {
public:
    KeyReleasedEvent(Key key)
        : Event(EventType::KeyReleased), key(key) {}

    Key key;
};

}