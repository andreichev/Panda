#pragma once

#include "Panda/Script.hpp"
#include "Manifest/ScriptFieldType.hpp"

#include <string>
#include <vector>

namespace Panda {

using InstantiateFunction = void *(*)(Entity entity);

struct ScriptFieldInfo final {
    constexpr ScriptFieldInfo(
        ScriptFieldType type, const char *name, uint32_t offset, uint32_t align
    )
        : type(type)
        , name(name)
        , offset(offset)
        , align(align) {}

    ScriptFieldType type;
    const char *name;
    uint32_t offset;
    uint32_t align;
};

struct ScriptClass final {
    const char *name;
    InstantiateFunction instantiateFunc;
    std::vector<ScriptFieldInfo> fields;
};

} // namespace Panda