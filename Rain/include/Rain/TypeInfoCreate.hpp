#pragma once

#include "TypeInfo.hpp"

namespace Rain {

template<typename T>
static TypeInfo createTypeInfo(TypeId id, std::string name) {
    TypeInfo info(id, std::move(name));
    info.size = sizeof(T);
    info.align = alignof(T);
    return info;
}

} // namespace Rain