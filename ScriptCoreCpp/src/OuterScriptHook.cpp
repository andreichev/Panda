//
// Created by Michael Andreichev on 02.08.2024.
//

#include "ScriptRegistry.hpp"
#include "OuterScriptHook.hpp"

#include <iostream>

namespace Panda {

namespace ExternalCalls {
    /// APPLICATION
    Application_Quit application_Quit = nullptr;
    Application_GetWidth application_GetWidth = nullptr;
    Application_GetHeight application_GetHeight = nullptr;
    /// WORLD
    World_Load world_Load = nullptr;
    World_FindByTag world_FindByTag = nullptr;
    World_CreateEntity world_CreateEntity = nullptr;
    World_DestroyEntity world_DestroyEntity = nullptr;
    /// INPUT
    Input_IsKeyPressed input_IsKeyPressed = nullptr;
    Input_IsKeyJustPressed input_IsKeyJustPressed = nullptr;
    /// CONSOLE
    Console_Log console_Log = nullptr;
} // namespace ExternalCalls

namespace InternalCalls {
    Panda::ScriptHandle addScriptFunc(UUID entityId, const char *name) {
        return Panda::getScriptRegistry()->instantiate(Entity(entityId), name);
    }

    void invokeUpdateAtScript(Panda::ScriptHandle handle, float deltaTime) {
        Panda::Script *script = Panda::getScriptRegistry()->getScriptWithId(handle);
        if (!script) {
            // assert(false);
            return;
        }
        script->update(deltaTime);
    }

    void invokeStartAtScript(Panda::ScriptHandle handle) {
        Panda::Script *script = Panda::getScriptRegistry()->getScriptWithId(handle);
        if (!script) {
            // assert(false);
            return;
        }
        script->start();
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
    g_scriptSymbols["addScriptFunc"] = (void *)addScriptFunc;
    g_scriptSymbols["invokeUpdateAtScript"] = (void *)invokeUpdateAtScript;
    g_scriptSymbols["invokeStartAtScript"] = (void *)invokeStartAtScript;
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
    application_Quit = (Application_Quit)load("application_Quit");
    application_GetWidth = (Application_GetWidth)load("application_GetWidth");
    application_GetHeight = (Application_GetHeight)load("application_GetHeight");
    input_IsKeyPressed = (Input_IsKeyPressed)load("input_IsKeyPressed");
    input_IsKeyJustPressed = (Input_IsKeyJustPressed)load("input_IsKeyJustPressed");
    world_Load = (World_Load)load("world_Load");
    world_FindByTag = (World_FindByTag)load("world_FindByTag");
    world_CreateEntity = (World_CreateEntity)load("world_CreateEntity");
    world_DestroyEntity = (World_DestroyEntity)load("world_DestroyEntity");
    console_Log = (Console_Log)load("console_Log");
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