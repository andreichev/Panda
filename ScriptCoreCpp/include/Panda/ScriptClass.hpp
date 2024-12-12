#pragma once

#include "Panda/Manifest/ScriptBundleManifest.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace Bamboo {
class Entity;
}

namespace Panda {

using InstantiateFunction = void *(*)(Bamboo::Entity entity);

struct ScriptFieldInfo final {
    constexpr ScriptFieldInfo(
        ScriptFieldType type, const char *name, uint32_t offset, uint32_t size, uint32_t align
    )
        : type(type)
        , name(name)
        , offset(offset)
        , size(size)
        , align(align) {}

    ScriptFieldType type;
    const char *name;
    uint32_t offset;
    uint32_t size;
    uint32_t align;
};

struct ScriptClass final {
    const char *name;
    InstantiateFunction instantiateFunc;
    std::unordered_map<FieldHandle, ScriptFieldInfo> fields;
};

} // namespace Panda