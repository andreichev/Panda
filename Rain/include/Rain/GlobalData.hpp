#pragma once

#include <unordered_map>

#include "TypeName.hpp"
#include "TypeInfoCreate.hpp"

namespace Rain {

class GlobalData {
public:
    template<typename T>
    TypeInfo &findOrCreateType() {
        std::string_view name = typeName<T>();
        const TypeId hash = std::hash<std::string_view>()(name);
        const auto it = data.find(hash);
        if (it == data.end()) {
            TypeInfo typeInfo = createTypeInfo<T>(hash, std::string(name));
            return data.emplace(hash, typeInfo).first->second;
        }
        return it->second;
    }

    TypeInfo &findInfo(TypeId id) {
        const auto it = data.find(id);
        // assert(it != data.end());
        return it->second;
    }

private:
    std::unordered_map<TypeId, TypeInfo> data;
};

GlobalData &getGlobalData();

TypeInfo &findInfo(TypeId id);

template<typename T>
constexpr TypeInfo &findOrCreateType() {
    return getGlobalData().findOrCreateType<T>();
}

template<typename FieldType>
constexpr FieldInfo &registerField(
    TypeInfo &classTypeInfo, const std::string &fieldName, uint32_t offset, uint32_t align
) {
    TypeInfo &fieldTypeInfo = findOrCreateType<FieldType>();
    TypeId fieldTypeId = fieldTypeInfo.id;

    using TypeRemovedExtents = std::remove_all_extents_t<FieldType>;
    using TypeRemovedRefs = std::remove_reference_t<TypeRemovedExtents>;
    using TypeRemovedPtrs = RemoveAllPointers<TypeRemovedRefs>;
    constexpr bool isRef{std::is_reference_v<FieldType>};
    constexpr bool isRValRef{std::is_rvalue_reference_v<FieldType>};
    constexpr bool isConst{std::is_const_v<TypeRemovedPtrs>};
    constexpr bool isVolatile{std::is_volatile_v<TypeRemovedPtrs>};

    FieldInfo &field = classTypeInfo.fields.emplace_back(fieldTypeId, fieldName, offset, align);

    if constexpr (isConst) {
        field.setConstFlag();
    }
    if constexpr (isVolatile) {
        field.setVolatileFlag();
    }
    if constexpr (isRef) {
        field.setReferenceFlag();
    }
    if constexpr (isRValRef) {
        field.setRValReferenceFlag();
    }

    return field;
}

template<typename ClassType, typename FieldType>
constexpr FieldInfo &registerField(const std::string &fieldName, uint32_t offset, uint32_t align) {
    TypeInfo &classTypeInfo = findOrCreateType<ClassType>();
    return registerField<FieldType>(classTypeInfo, fieldName, offset, align);
}

} // namespace Rain
