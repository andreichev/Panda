#pragma once

#include "Panda/Component.hpp"
#include "Panda/Entity.hpp"

namespace Panda {

class Script : public Component {
public:
    virtual ~Script() = default;

    virtual void start() {};
    virtual void update(float deltaTime) {};

private:
    ScriptClassHandle m_classHandle;
    friend class ScriptRegistry;
};

} // namespace Panda