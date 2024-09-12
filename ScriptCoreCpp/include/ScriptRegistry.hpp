#pragma once

#include "ScriptCore.hpp"

#include <vector>
#include <unordered_map>

namespace Panda {

using ScriptHandle = uint32_t;

using InstantiateFunction = void *(*)();

struct ScriptClassContainer {
    const char *name;
    InstantiateFunction instantiateFunc;

    ScriptClassContainer(const char *name, InstantiateFunction instantiateFunc)
        : name(name)
        , instantiateFunc(instantiateFunc) {}
};

class ScriptRegistry {
public:
    std::vector<ScriptClassContainer> m_scriptClasses;
    std::unordered_map<ScriptHandle, Script *> m_instances;

    template<typename T>
    void registerScriptClass(const char *name) {
        m_scriptClasses.emplace_back(name, []() { return (void *)new T(); });
    }

    Script *getScriptWithId(ScriptHandle id) {
        if (m_instances.find(id) == m_instances.end()) {
            return nullptr;
        }
        return m_instances.at(id);
    }

    void removeScriptId(ScriptHandle id) {
        if (m_instances.find(id) == m_instances.end()) {
            return;
        }
        delete m_instances.at(id);
        m_instances.erase(id);
    }

    ScriptHandle instantiateWithName(const char *name) {
        for (auto clazz : m_scriptClasses) {
            if (strcmp(name, clazz.name) == 0) {
                m_lastHandle++;
                m_instances[m_lastHandle] = (Script *)clazz.instantiateFunc();
                return m_lastHandle;
            }
        }
        return 0;
    }

    ~ScriptRegistry() {
        for (auto instance : m_instances) {
            delete instance.second;
        }
    }

private:
    ScriptHandle m_lastHandle = 0;
};

ScriptRegistry *getScriptRegistry();

template<typename T>
struct AutoRegisterScriptClass {
    AutoRegisterScriptClass(const char *name) {
        Panda::getScriptRegistry()->registerScriptClass<T>(name);
    }
};

#define _PANDA_CONCAT_(a, b) a##b
#define _REGISTER_SCRIPT_INTERNAL(name, ID)                                                        \
    inline static Panda::AutoRegisterScriptClass<name> _PANDA_CONCAT_(registerScript_, ID)(#name);
#define REGISTER_SCRIPT(name) _REGISTER_SCRIPT_INTERNAL(name, __COUNTER__)

}; // namespace Panda
