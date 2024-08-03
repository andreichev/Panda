//
// Created by Michael Andreichev on 01.08.2024.
//

#pragma once

namespace Panda {

struct ScriptEngineConfig {
    std::filesystem::path scriptsDllPath;
};

class ScriptEngine {
public:
    static bool init(ScriptEngineConfig config);
    static void terminate();

private:
    static void *lib;
};

} // namespace Panda