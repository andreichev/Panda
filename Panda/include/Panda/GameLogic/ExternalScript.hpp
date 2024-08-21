#pragma once

namespace Panda {

class ExternalScript {
public:
    ExternalScript(uint32_t id)
        : id(id) {}

    void invokeUpdate(float deltaTime) {

    };

private:
    uint32_t id;
};

} // namespace Panda