//
// Created by Michael Andreichev on 01.08.2024.
//

#include "Panda/ScriptEngine/ScriptEngine.hpp"
#include "Panda/ScriptEngine/InnerScriptHook.hpp"

#include <Foundation/Foundation.hpp>
#include <dylib.hpp>

namespace Panda {

ScriptEngine::~ScriptEngine() {
    terminate();
}

void ScriptEngine::terminate() {
    if (lib) {
        F_DELETE(Foundation::getAllocator(), (dylib *)lib);
        lib = nullptr;
    }
}

bool ScriptEngine::reload(ScriptEngineConfig config) {
    terminate();
    try {
        dylib *pDylib = F_NEW(Foundation::getAllocator(), dylib)(
            config.dllPath.c_str(), config.dllName.c_str(), true
        );
        lib = pDylib;
        // ------------
        // OUTER LOADER
        // ------------
        auto outerExternalLoader =
            pDylib->get_function<int(Panda::SymbolsLoadFunc load)>("loadExternalCalls");
        outerExternalLoader(loadInternalCall);

        // ------------
        // INNER LOADER
        // ------------
        auto outerInternalLoader = pDylib->get_function<void *(const char *)>("loadInternalCall");
        loadExternalCalls(outerInternalLoader);
    } catch (...) {
        LOG_ERROR(
            "SCRIPT ENGINE: Can't load script dynamic library at path {}/{}",
            config.dllPath.string().c_str(),
            config.dllName.c_str()
        );
        return false;
    }
    return true;
}

} // namespace Panda
