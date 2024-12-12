//
// Created by Michael Andreichev on 01.08.2024.
//

#pragma once

#include <Panda/ScriptEngine/Manifest/ScriptBundleManifest.hpp>
#include <filesystem>

namespace Panda {

struct ScriptEngineConfig {
    std::filesystem::path dllPath;
    std::string dllName;
};

class ScriptEngine {
public:
    ScriptEngine();
    ~ScriptEngine();
    bool reload(ScriptEngineConfig config);
    bool isLoaded();
    ScriptBundleManifest getManifest();
    void terminate();

private:
    ScriptBundleManifest m_manifest;
    void *m_lib;
};

} // namespace Panda