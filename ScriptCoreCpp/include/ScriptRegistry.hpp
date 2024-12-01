#pragma once

#include "ScriptClass.hpp"

#include <unordered_map>

namespace Panda {

using ScriptHandle = uint32_t;

class ScriptRegistry {
private:
    template<typename T>
    static auto addFieldsIfHas(ScriptClass &) -> std::enable_if_t<!hasFields<T>> {}

    template<typename T>
    static auto addFieldsIfHas(ScriptClass &info) -> std::enable_if_t<hasFields<T>> {
        info.fields = StripType<T>::getFields();
    }

public:
    std::vector<ScriptClass> m_scriptClasses;
    std::unordered_map<ScriptHandle, Script *> m_instances;

    template<typename T>
    ScriptFieldType getType() {
        return ScriptFieldType::UNKNOWN;
    }

    template<>
    ScriptFieldType getType<int>() {
        return ScriptFieldType::INTEGER;
    }

    template<typename FieldType>
    ScriptFieldInfo makeFieldInfo(const char *fieldName, uint32_t offset) {
        ScriptFieldType type = getType<FieldType>();

        using TypeRemovedExtents = std::remove_all_extents_t<FieldType>;
        using TypeRemovedRefs = std::remove_reference_t<TypeRemovedExtents>;
        using TypeRemovedPtrs = RemoveAllPointers<TypeRemovedRefs>;
        constexpr bool isRef = std::is_reference_v<FieldType>;
        constexpr bool isConst = std::is_const_v<TypeRemovedPtrs>;

        static_assert(!isConst, "Const types in script fields not supported");
        static_assert(!isRef, "Reference types in script fields not supported");

        return ScriptFieldInfo(type, fieldName, offset, alignof(FieldType));
    }

    template<typename T>
    void registerScriptClass(const char *name) {
        ScriptClass clazz;
        clazz.name = name;
        addFieldsIfHas<T>(clazz);
        clazz.instantiateFunc = [](Entity entity) {
            T *script = new T();
            script->m_entity = entity;
            return (void *)script;
        };
        m_scriptClasses.emplace_back(clazz);
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

    void removeAllScripts() {
        for (auto instance : m_instances) {
            delete instance.second;
        }
        m_instances.clear();
    }

    ScriptHandle instantiate(Entity entity, const char *name) {
        for (auto clazz : m_scriptClasses) {
            if (strcmp(name, clazz.name) == 0) {
                m_lastHandle++;
                m_instances[m_lastHandle] = (Script *)clazz.instantiateFunc(entity);
                return m_lastHandle;
            }
        }
        return 0;
    }

    ~ScriptRegistry() {
        removeAllScripts();
    }

private:
    ScriptHandle m_lastHandle = 0;
};

ScriptRegistry *getScriptRegistry();

} // namespace Panda
