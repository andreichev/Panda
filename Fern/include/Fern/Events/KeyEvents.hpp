//
// Created by Admin on 19.02.2022.
//

#include "Fern/Events/Event.hpp"
#include "Fern/Events/Key.hpp"

namespace Fern {

struct CharacterInputEvent : public Event {
    CharacterInputEvent(uint32_t c)
        : Event(EventType::InputCharacter)
        , c(c) {}

    uint32_t c;
};

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

} // namespace Fern