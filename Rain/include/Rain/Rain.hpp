#pragma once

#include "TypeRegistry.hpp"

// --------------------------------
// ---------- MACROS --------------
// --------------------------------

#define RAIN_FIELDS_BEGIN(classType)                                                               \
    static std::vector<Rain::FieldInfo> getFields() {                                              \
        using ClassType = classType;                                                               \
        auto fields = std::vector<Rain::FieldInfo>();

#define RAIN_FIELD(name)                                                                           \
    static_assert(                                                                                 \
        !Rain::stringsEqual(#name, "fields"), "Incorrect name \"fields\". Plrease, rename"         \
    );                                                                                             \
    fields.emplace_back(Rain::getTypeRegistry()->makeFieldInfo<decltype(name)>(                    \
        #name, Rain::offsetOf(&ClassType::name)                                                    \
    ));

#define RAIN_FIELDS_END                                                                            \
    return fields;                                                                                 \
    }

#include "Codable.hpp"