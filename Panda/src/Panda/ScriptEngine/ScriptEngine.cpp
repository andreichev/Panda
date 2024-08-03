//
// Created by Michael Andreichev on 01.08.2024.
//

#include "ScriptEngine.hpp"
#include "ScriptGlue.hpp"

#include <Foundation/Foundation.hpp>
#include <dylib.hpp>

namespace Panda {

void *ScriptEngine::lib = nullptr;

bool ScriptEngine::init(ScriptEngineConfig config) {
    try {
        dylib *pDylib = NEW(Foundation::getAllocator(), dylib)(config.scriptsDllPath.c_str());
        auto loader = pDylib->get_function<int(PandaScriptHook::ScriptsLoadFunc load)>(
            "scriptSymbolsBindWithLoader"
        );
        loader(loadScriptFunc);
        lib = pDylib;
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
