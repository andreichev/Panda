//
// Created by Michael Andreichev on 01.08.2024.
//

#pragma once

#include <filesystem>

namespace Panda {

struct ScriptEngineConfig {
    std::filesystem::path dllPath;
    std::string dllName;
};

class ScriptEngine {
public:
    ~ScriptEngine();
    bool reload(ScriptEngineConfig config);
    void terminate();

private:
    void *lib = nullptr;
};

} // namespace Panda