#pragma once

#include <Panda/ScriptEngine/InnerScriptHook.hpp>

namespace Panda {

class ExternalScript {
public:
    ExternalScript(uint32_t id, const char *name)
        : id(id)
        , name(name) {}

    void invokeUpdate(float deltaTime) {
        ExternalCalls::invokeUpdateAtScriptFunc(id, deltaTime);
    };

    const char *getName() {
        return name;
    }

private:
    uint32_t id;
    const char *name;
};

} // namespace Panda