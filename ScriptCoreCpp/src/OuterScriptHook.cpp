//
// Created by Michael Andreichev on 02.08.2024.
//

#include "ScriptRegistry.hpp"

#include <iostream>

namespace Panda {

namespace ExternalCalls {
    ApplicationQuit applicationQuit = nullptr;

} // namespace ExternalCalls

namespace InternalCalls {
    Panda::ScriptHandle createScriptWithName(const char *name) {
        return Panda::getScriptRegistry()->instantiateWithName(name);
    }

    void invokeUpdateAtScript(Panda::ScriptHandle handle, float deltaTime) {
        Panda::Script *script = Panda::getScriptRegistry()->getScriptWithId(handle);
        if (!script) {
            // assert(false);
            return;
        }
        script->update(deltaTime);
    }

    std::vector<const char *> getAvailableScripts() {
        std::vector<const char *> result;
        for (auto &clazz : getScriptRegistry()->m_scriptClasses) {
            result.push_back(clazz.name);
        }
        return result;
    }
} // namespace InternalCalls

std::unordered_map<std::string, void *> g_scriptSymbols;

void initScriptHook() {
    using namespace InternalCalls;
    g_scriptSymbols["createScriptWithName"] = (void *)createScriptWithName;
    g_scriptSymbols["invokeUpdateAtScript"] = (void *)invokeUpdateAtScript;
    g_scriptSymbols["getAvailableScripts"] = (void *)getAvailableScripts;
}

//////////////////////////////////////////////////////////////////
///---------------------Functions binding----------------------///
//////////////////////////////////////////////////////////////////

#if defined(_WIN32) || defined(_WIN64)
#    define LIB_EXPORT __declspec(dllexport)
#else
#    define LIB_EXPORT
#endif

extern "C" {

using SymbolsLoadFunc = void *(*)(const char *name);

LIB_EXPORT int loadExternalCalls(SymbolsLoadFunc load) {
    using namespace ExternalCalls;
    applicationQuit = (ApplicationQuit)load("applicationQuit");
    std::cout << "SCRIPT ENGINE: Outer functions binding done.\n";
    return 0;
}

LIB_EXPORT void *loadInternalCall(const char *name) {
    if (g_scriptSymbols.empty()) {
        initScriptHook();
    }
    if (g_scriptSymbols.find(name) == g_scriptSymbols.end()) {
        std::cerr << "SCRIPT ENGINE ERROR: Outer function not found: " << name << "\n";
        return nullptr;
    }
    return g_scriptSymbols.at(name);
}

LIB_EXPORT void hookFunc() {
    loadExternalCalls(nullptr);
    loadInternalCall(nullptr);
}
}

} // namespace Panda