#pragma once

#include "Bamboo/Base.hpp"
#include "Panda/Base.hpp"

namespace Bamboo {

class Component;

class Entity final {
public:
    Entity();
    Entity(Panda::EntityHandle id);

    template<typename T>
        requires std::is_base_of_v<Component, T>
    T getComponent() {
        T component;
        component.m_entity = *this;
        return component;
    }

    inline bool isValid() {
        return m_id != 0;
    }

    inline uint32_t getId() {
        return m_id;
    }

private:
    Panda::EntityHandle m_id;

    friend class World;
};

} // namespace Bamboo