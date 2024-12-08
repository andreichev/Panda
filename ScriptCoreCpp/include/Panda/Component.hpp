#pragma once

#include "Entity.hpp"

namespace Panda {

class Component {
public:
    Entity getEntity() {
        return m_entity;
    }

private:
    Entity m_entity;

    friend class ScriptRegistry;
};

} // namespace Panda