#include "Panda/ScriptEngine/InnerScriptHook.hpp"
#include "Panda/Application/Application.hpp"
#include "Panda/GameLogic/Input.hpp"

#include <unordered_map>

namespace Panda {

namespace InternalCalls {
    /// APPLICATION
    void application_Quit() {
        Panda::Application::get()->close();
    }

    uint32_t application_GetWidth() {
        return Panda::Application::get()->getWindow()->getSize().width;
    }

    uint32_t application_GetHeight() {
        return Panda::Application::get()->getWindow()->getSize().height;
    }

    /// INPUT
    bool input_IsKeyPressed(int key) {
        return Input::isKeyPressed((Key)key);
    }

    bool input_IsKeyJustPressed(int key) {
        return Input::isKeyJustPressed((Key)key);
    }

    /// WORLD
    void world_Load(const char *name) {
        // TODO: Implement using AssetManager
    }

    /// CONSOLE
    void console_Log(const char *message) {
        LOG_EDITOR(message);
    }
} // namespace InternalCalls

std::unordered_map<std::string, void *> g_scriptSymbols;

void initScriptHook() {
    g_scriptSymbols["application_Quit"] = (void *)InternalCalls::application_Quit;
    g_scriptSymbols["application_GetWidth"] = (void *)InternalCalls::application_GetWidth;
    g_scriptSymbols["application_GetHeight"] = (void *)InternalCalls::application_GetHeight;
    g_scriptSymbols["input_IsKeyPressed"] = (void *)InternalCalls::input_IsKeyPressed;
    g_scriptSymbols["input_IsKeyJustPressed"] = (void *)InternalCalls::input_IsKeyJustPressed;
    g_scriptSymbols["world_Load"] = (void *)InternalCalls::world_Load;
    g_scriptSymbols["console_Log"] = (void *)InternalCalls::console_Log;
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