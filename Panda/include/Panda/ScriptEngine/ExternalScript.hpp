#pragma once

#include <Foundation/Memory.hpp>
#include <Panda/ScriptEngine/InnerScriptHook.hpp>

namespace Panda {

struct ScriptField {
    ScriptInstanceHandle instanceId;
    FieldHandle fieldId;
    std::string name;
    ScriptFieldType type;
    Foundation::Memory data;

    ScriptField(
        ScriptInstanceHandle instanceId,
        FieldHandle fieldId,
        const std::string &name,
        ScriptFieldType type
    )
        : instanceId(instanceId)
        , fieldId(fieldId)
        , name(name)
        , type(type)
        , data() {
        switch (type) {
            case ScriptFieldType::INTEGER: {
                data = Foundation::Memory::alloc(sizeof(int));
                break;
            }
            default: {
                break;
            }
        }
    }
};

class ExternalScript {
public:
    ExternalScript(uint32_t id, const std::string &name, std::vector<ScriptField> fields)
        : id(id)
        , name(name)
        , fields(fields) {}

    void invokeStart() {
        ExternalCalls::invokeStartAtScript(id);
    }

    void invokeUpdate(float deltaTime) {
        ExternalCalls::invokeUpdateAtScript(id, deltaTime);
    };

    const std::string &getName() {
        return name;
    }

    std::vector<ScriptField> &getFields() {
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