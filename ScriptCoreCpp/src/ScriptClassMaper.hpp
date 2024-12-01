#pragma once

#include "Manifest/ScriptClassManifest.hpp"
#include "ScriptClass.hpp"

namespace Panda {

class ScriptClassMapper {
public:
    static ScriptFieldManifest getFieldManifest(ScriptFieldInfo field) {
        return ScriptFieldManifest(field.name, field.type);
    }

    static std::vector<ScriptFieldManifest> getFieldsManifest(std::vector<ScriptFieldInfo> fields) {
        std::vector<ScriptFieldManifest> manifest;
        for (auto &field : fields) {
            manifest.push_back(getFieldManifest(field));
        }
        return manifest;
    }

    static ScriptClassManifest getClassManifest(ScriptClass clazz) {
        ScriptClassManifest manifest;
        manifest.name = clazz.name;
        manifest.fields = getFieldsManifest(clazz.fields);
        return manifest;
    }

    static std::vector<ScriptClassManifest> getClassesManifest(std::vector<ScriptClass> classes) {
        std::vector<ScriptClassManifest> manifest;
        for (auto &clazz : classes) {
            manifest.push_back(getClassManifest(clazz));
        }
        return manifest;
    }
};

} // namespace Panda