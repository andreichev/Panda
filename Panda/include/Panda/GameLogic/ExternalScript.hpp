#pragma once

#include <Panda/ScriptEngine/InnerScriptHook.hpp>

namespace Panda {

class ExternalScript {
public:
    ExternalScript(uint32_t id, const std::string &name)
        : id(id)
        , name(name) {}

    void invokeUpdate(float deltaTime) {
        ExternalCalls::invokeUpdateAtScriptFunc(id, deltaTime);
    };

    const std::string &getName() {
        return name;
    }

    bool operator==(const ExternalScript &rhs) const {
        return id == rhs.id;
    }

private:
    uint32_t id;
    std::string name;
};

} // namespace Panda