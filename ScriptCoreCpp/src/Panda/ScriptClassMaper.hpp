#pragma once

#include "Panda/Manifest/ScriptBundleManifest.hpp"
#include "Panda/ScriptClass.hpp"

namespace Panda {

class ScriptClassMapper {
public:
    static ScriptFieldManifest getFieldManifest(FieldHandle handle, ScriptFieldInfo field) {
        return ScriptFieldManifest(handle, field.name, field.type);
    }

    static std::vector<ScriptFieldManifest>
    getFieldsManifest(std::unordered_map<FieldHandle, ScriptFieldInfo> fields) {
        std::vector<ScriptFieldManifest> manifest;
        for (auto &field : fields) {
            manifest.push_back(getFieldManifest(field.first, field.second));
        }
        return manifest;
    }

    static ScriptClassManifest getClassManifest(ScriptClass clazz) {
        ScriptClassManifest manifest;
        manifest.name = clazz.name;
        manifest.fields = getFieldsManifest(clazz.fields);
        return manifest;
    }

    static ScriptBundleManifest getClassesManifest(std::vector<ScriptClass> classes) {
        std::vector<ScriptClassManifest> manifest;
        for (auto &clazz : classes) {
            manifest.push_back(getClassManifest(clazz));
        }
        return {manifest};
    }
};

} // namespace Panda