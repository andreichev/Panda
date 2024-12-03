#pragma once

#include "ScriptFieldType.hpp"

#include <vector>

namespace Panda {

struct ScriptFieldManifest final {
    constexpr ScriptFieldManifest(FieldHandle handle, const char *name, ScriptFieldType type)
        : handle(handle)
        , name(name)
        , type(type) {}

    FieldHandle handle;
    const char *name;
    ScriptFieldType type;
};

struct ScriptClassManifest final {
    const char *name;
    std::vector<ScriptFieldManifest> fields;
};

} // namespace Panda