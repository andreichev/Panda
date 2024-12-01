#pragma once

#include "ScriptFieldType.hpp"

#include <vector>

namespace Panda {

struct ScriptFieldManifest final {
    constexpr ScriptFieldManifest(const char *name, ScriptFieldType type)
        : name(name)
        , type(type) {}

    const char *name;
    ScriptFieldType type;
};

struct ScriptClassManifest final {
    const char *name;
    std::vector<ScriptFieldManifest> fields;
};

} // namespace Panda