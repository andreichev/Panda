#pragma once

#include <unordered_map>

#include "TypeName.hpp"
#include "TypeInfo.hpp"

namespace Rain {

class GlobalData {
public:
    template<typename T>
    TypeInfo &findOrCreateType() {
        std::string_view name = typeName<T>();
        const TypeId hash = std::hash<std::string_view>()(name);
        const auto it = data.find(hash);
        if (it == data.end()) {
            TypeInfo typeInfo = TypeInfo::create<T>(hash, name);
            return data.emplace(hash, typeInfo).first->second;
        }
        return it->second;
    }

private:
    std::unordered_map<TypeId, TypeInfo> data;
};

GlobalData &getGlobalData();

template<typename T>
constexpr TypeInfo &findOrCreateType() {
    return getGlobalData().findOrCreateType<T>();
}

template<typename ClassType, typename FieldType>
constexpr FieldInfo &registerField(std::string_view fieldName, uint32_t offset, uint32_t align) {
    GlobalData &data = getGlobalData();
    TypeInfo &classTypeInfo = data.findOrCreateType<ClassType>();
    TypeInfo &fieldTypeInfo = data.findOrCreateType<FieldType>();
    TypeId fieldTypeId = fieldTypeInfo.id;
    return classTypeInfo.fields.emplace_back(fieldTypeId, fieldName, offset, align);
}

template<typename FieldType>
constexpr FieldInfo &registerField(
    TypeInfo &classTypeInfo, std::string_view fieldName, uint32_t offset, uint32_t align
) {
    GlobalData &data = getGlobalData();
    TypeInfo &fieldTypeInfo = data.findOrCreateType<FieldType>();
    TypeId fieldTypeId = fieldTypeInfo.id;
    return classTypeInfo.fields.emplace_back(fieldTypeId, fieldName, offset, align);
}

} // namespace Rain
