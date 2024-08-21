#pragma once

namespace Panda {

class Script {
public:
    virtual ~Script() = default;
    virtual void init() {};
    virtual void update(float deltaTime) {};
};

} // namespace Panda