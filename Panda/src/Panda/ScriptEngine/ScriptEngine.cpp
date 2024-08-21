//
// Created by Michael Andreichev on 01.08.2024.
//

#include "ScriptEngine.hpp"
#include "InnerScriptHook.hpp"

#include <Foundation/Foundation.hpp>
#include <dylib.hpp>

namespace Panda {

void *ScriptEngine::lib = nullptr;

bool ScriptEngine::init(ScriptEngineConfig config) {
    try {
        dylib *pDylib = NEW(Foundation::getAllocator(), dylib)(config.scriptsDllPath.c_str());
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
    } catch (...) { return false; }
    return true;
}

void ScriptEngine::terminate() {
    if (lib) {
        DELETE(Foundation::getAllocator(), (dylib *)lib);
        lib = nullptr;
    }
}

} // namespace Panda
