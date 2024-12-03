#pragma once

#include <string_view>
#include <unordered_map>

#include "ScriptRegistry.hpp"

namespace Panda {

template<typename T>
struct AutoRegisterScriptClass {
    AutoRegisterScriptClass(const char *name) {
        Panda::getScriptRegistry()->registerScriptClass<T>(name);
    }
};

} // namespace Panda

// --------------------------------
// ---------- MACROS --------------
// --------------------------------

#define _PANDA_CONCAT_(a, b) a##b
#define _REGISTER_SCRIPT_INTERNAL(name, ID)                                                        \
    inline static Panda::AutoRegisterScriptClass<name> _PANDA_CONCAT_(registerScript_, ID)(#name);
#define REGISTER_SCRIPT(name) _REGISTER_SCRIPT_INTERNAL(name, __COUNTER__)

#define PANDA_FIELDS_BEGIN(classType)                                                              \
    static std::unordered_map<Panda::FieldHandle, Panda::ScriptFieldInfo> getFields() {            \
        using ClassType = classType;                                                               \
        std::unordered_map<Panda::FieldHandle, Panda::ScriptFieldInfo> fields;

#define PANDA_FIELD(name)                                                                          \
    fields.emplace(                                                                                \
        std::hash<std::string_view>()(#name),                                                      \
        Panda::getScriptRegistry()->makeFieldInfo<decltype(name)>(                                 \
            #name, Panda::offsetOf(&ClassType::name)                                               \
        )                                                                                          \
    );

#define PANDA_FIELDS_END                                                                           \
    return fields;                                                                                 \
    }