#pragma once

namespace Panda {

using ScriptHandle = uint32_t;

namespace InternalCalls {
    void application_Quit();
} // namespace InternalCalls

namespace ExternalCalls {
    using AddScriptFunc = ScriptHandle (*)(const char *name);
    extern AddScriptFunc addScriptFunc;

    using InvokeUpdateAtScriptFunc = void (*)(Panda::ScriptHandle handle, float deltaTime);
    extern InvokeUpdateAtScriptFunc invokeUpdateAtScriptFunc;
} // namespace ExternalCalls

//////////////////////////////////////////////////////////////////
///---------------------Functions binding----------------------///
//////////////////////////////////////////////////////////////////

using SymbolsLoadFunc = void *(*)(const char *name);

void *loadInternalCall(const char *name);
int loadExternalCalls(SymbolsLoadFunc load);

} // namespace Panda