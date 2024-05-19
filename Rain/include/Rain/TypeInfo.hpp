#pragma once

#include "Rain/Base.hpp"

namespace Rain {

struct FieldInfo final {
    TypeId typeId;
    std::string_view name;
    uint32_t offset;
    uint32_t align;

    FieldInfo(TypeId typeId, std::string_view name, uint32_t offset, uint32_t align)
        : typeId(typeId)
        , name(name)
        , offset(offset)
        , align(align) {}
};

struct TypeInfo final {
    TypeId id;
    std::string_view name;
    uint32_t size;
    uint32_t align;
    std::vector<FieldInfo> fields;

    template<typename T>
    static TypeInfo create(TypeId id, std::string_view name) {
        TypeInfo info(id, name);
        info.size = sizeof(T);
        info.align = alignof(T);
        return info;
    }

private:
    explicit TypeInfo(TypeId id, std::string_view name)
        : id(id)
        , name(name)
        , size(0)
        , align(0)
        , fields() {}
};

} // namespace Rain
