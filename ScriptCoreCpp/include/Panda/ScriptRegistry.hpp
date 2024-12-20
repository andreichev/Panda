#pragma once

#include "ScriptClass.hpp"
#include "Panda/Base.hpp"
#include "Bamboo/Script.hpp"

#include <unordered_map>

namespace Panda {

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
    std::unordered_map<ScriptInstanceHandle, Bamboo::Script *> m_instances;

    template<typename T>
    ScriptFieldType getType() {
        static_assert(false, "Unknown field type");
        return ScriptFieldType::UNKNOWN;
    }

    template<>
    ScriptFieldType getType<int>() {
        return ScriptFieldType::INTEGER;
    }

    template<>
    ScriptFieldType getType<Bamboo::Entity>() {
        return ScriptFieldType::ENTITY;
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

        return ScriptFieldInfo(type, fieldName, offset, sizeof(FieldType), alignof(FieldType));
    }

    template<typename T>
    void registerScriptClass(const char *name) {
        ScriptClass clazz;
        clazz.name = name;
        addFieldsIfHas<T>(clazz);
        clazz.instantiateFunc = [](Bamboo::Entity entity) {
            // TODO: Use custom allocator for script instances
            T *script = new T();
            script->m_entity = entity;
            return (void *)script;
        };
        m_scriptClasses.emplace_back(clazz);
    }

    Bamboo::Script *getInstanceWithId(ScriptInstanceHandle id) {
        if (m_instances.find(id) == m_instances.end()) {
            return nullptr;
        }
        return m_instances.at(id);
    }

    void setFieldValue(ScriptInstanceHandle scriptId, FieldHandle fieldId, void *value) {
        Bamboo::Script *script = getInstanceWithId(scriptId);
        // PND_ASSERT(script, "Invalid script instance id");
        ScriptClassHandle classHandle = script->m_classHandle;
        // PND_ASSERT(classHandle >= 0 && classHandle < m_scriptClasses.size(), "Invalid class
        // handle");
        ScriptClass &clazz = m_scriptClasses[classHandle];
        // PND_ASSERT(clazz.fields.find(fieldId) != clazz.fields.end(), "Invalid field id");
        ScriptFieldInfo &info = clazz.fields.at(fieldId);
        void *ptr = addOffset(script, info.offset);
        memcpy(ptr, value, info.size);
    }

    void removeScriptId(ScriptInstanceHandle id) {
        if (m_instances.find(id) == m_instances.end()) {
            return;
        }
        delete m_instances.at(id);
        m_instances.erase(id);
    }

    void clear() {
        for (auto instance : m_instances) {
            delete instance.second;
        }
        m_instances.clear();
    }

    ScriptInstanceHandle instantiate(Bamboo::Entity entity, const char *name) {
        for (ScriptClassHandle classId = 0; classId < m_scriptClasses.size(); classId++) {
            ScriptClass &clazz = m_scriptClasses[classId];
            if (strcmp(name, clazz.name) == 0) {
                m_lastHandle++;
                m_instances[m_lastHandle] = (Bamboo::Script *)clazz.instantiateFunc(entity);
                m_instances[m_lastHandle]->m_classHandle = classId;
                return m_lastHandle;
            }
        }
        return 0;
    }

    ~ScriptRegistry() {
        clear();
    }

private:
    ScriptClassHandle m_lastHandle = 0;
};

ScriptRegistry *getScriptRegistry();

} // namespace Panda
