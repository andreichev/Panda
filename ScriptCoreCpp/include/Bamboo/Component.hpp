#pragma once

#include "Entity.hpp"

namespace Panda {
class ScriptRegistry;
}

namespace Bamboo {

class Component {
public:
    Entity getEntity();

private:
    Entity m_entity;

    friend class Entity;
    friend class Panda::ScriptRegistry;
};

} // namespace Bamboo