#pragma once

#include "Bamboo/Component.hpp"
#include "Bamboo/Entity.hpp"
#include "Panda/Base.hpp"

namespace Panda {
class ScriptRegistry;
}

namespace Bamboo {

class Script : public Component {
public:
    virtual ~Script() = default;

    virtual void start() {};
    virtual void update(float deltaTime) {};
    virtual void beginCollisionTouch(Entity other) {};
    virtual void endCollisionTouch(Entity other) {};
    virtual void beginSensorOverlap(Entity sensor) {};
    virtual void endSensorOverlap(Entity sensor) {};

private:
    Panda::ScriptClassHandle m_classHandle;
    friend class Panda::ScriptRegistry;
};

} // namespace Bamboo