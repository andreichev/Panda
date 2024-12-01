#pragma once

#include <Panda/ScriptEngine/InnerScriptHook.hpp>

namespace Panda {

struct ScriptField {
    std::string name;
    ScriptFieldType type;

    ScriptField(const std::string &name, ScriptFieldType type)
        : name(name)
        , type(type) {}
};

class ExternalScript {
public:
    ExternalScript(uint32_t id, const std::string &name, std::vector<ScriptField> fields)
        : id(id)
        , name(name)
        , fields(fields) {}

    void invokeStart() {
        ExternalCalls::invokeStartAtScriptFunc(id);
    }

    void invokeUpdate(float deltaTime) {
        ExternalCalls::invokeUpdateAtScriptFunc(id, deltaTime);
    };

    const std::string &getName() {
        return name;
    }

    [[nodiscard]] std::vector<ScriptField> getFields() const {
        return fields;
    }

    bool operator==(const ExternalScript &rhs) const {
        return id == rhs.id;
    }

    void rebindId(uint32_t _id) {
        id = _id;
    }

private:
    uint32_t id;
    std::string name;
    std::vector<ScriptField> fields;
};

} // namespace Panda