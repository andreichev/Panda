#include "Panda/ScriptEngine/InnerScriptHook.hpp"
#include "Panda/Application/Application.hpp"

#include <unordered_map>

namespace Panda {

namespace InternalCalls {
    void applicationQuit() {
        Panda::Application::get()->close();
    }
} // namespace InternalCalls

std::unordered_map<std::string, void *> g_scriptSymbols;

void initScriptHook() {
    g_scriptSymbols["applicationQuit"] = (void *)InternalCalls::applicationQuit;
}

namespace ExternalCalls {
    AddScriptFunc addScriptFunc = nullptr;
    InvokeUpdateAtScriptFunc invokeUpdateAtScriptFunc = nullptr;
    GetAvailableScripts getAvailableScripts = nullptr;
} // namespace ExternalCalls

//////////////////////////////////////////////////////////////////
//---------------------Functions binding------------------------//
//////////////////////////////////////////////////////////////////

void *loadInternalCall(const char *name) {
    if (g_scriptSymbols.empty()) {
        initScriptHook();
    }
    if (g_scriptSymbols.find(name) == g_scriptSymbols.end()) {
        return nullptr;
    }
    return g_scriptSymbols.at(name);
}

using SymbolsLoadFunc = void *(*)(const char *name);

int loadExternalCalls(SymbolsLoadFunc load) {
    using namespace ExternalCalls;
    addScriptFunc = (AddScriptFunc)load("createScriptWithName");
    invokeUpdateAtScriptFunc = (InvokeUpdateAtScriptFunc)load("invokeUpdateAtScript");
    getAvailableScripts = (GetAvailableScripts)load("getAvailableScripts");
    LOG_INFO("SCRIPT ENGINE: Inner functions binding done.");
    return 0;
}

} // namespace Panda