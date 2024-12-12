#pragma once

#include <Foundation/Memory.hpp>
#include <Panda/ScriptEngine/InnerScriptHook.hpp>

namespace Panda {

struct ScriptField {
    ScriptInstanceHandle instanceId;
    FieldHandle fieldId;
    std::string name;
    ScriptFieldType type;
    Foundation::Memory value;

    ScriptField(
        ScriptInstanceHandle instanceId,
        FieldHandle fieldId,
        const std::string &name,
        ScriptFieldType type,
        Foundation::Memory value
    )
        : instanceId(instanceId)
        , fieldId(fieldId)
        , name(name)
        , type(type)
        , value(value) {}

    operator bool() {
        return value.data;
    }

    bool operator==(const ScriptField &other) const {
        return instanceId == other.instanceId && fieldId == other.fieldId;
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
    }

    const std::string &getName() {
        return name;
    }

    bool hasField(const char *name) {
        return findField(name) != fields.end();
    }

    ScriptField &getField(const char *name) {
        return *findField(name);
    }

    std::vector<ScriptField> &getFields() {
        return fields;
    }

    void addField(ScriptField field) {
        fields.emplace_back(field);
    }

    void removeField(ScriptField &field) {
        auto it = std::remove(fields.begin(), fields.end(), field);
        if (it != fields.end()) {
            fields.erase(it);
        }
    }

    bool operator==(const ExternalScript &rhs) const {
        return id == rhs.id;
    }

    void rebindId(uint32_t _id) {
        id = _id;
    }

private:
    std::vector<ScriptField>::iterator findField(const char *name) {
        return std::find_if(fields.begin(), fields.end(), [&](const ScriptField &item) {
            return item.name == name;
        });
    }

    uint32_t id;
    std::string name;
    std::vector<ScriptField> fields;
};

} // namespace Panda