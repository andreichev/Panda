#pragma once

#include <stdint.h>
#include <Rain/UUID.hpp>

namespace Panda {

using ScriptHandle = uint32_t;

namespace ExternalCalls {
    using AddScriptFunc = ScriptHandle (*)(UUID entityId, const char *name);
    extern AddScriptFunc addScriptFunc;

    using InvokeUpdateAtScriptFunc = void (*)(ScriptHandle handle, float deltaTime);
    extern InvokeUpdateAtScriptFunc invokeUpdateAtScriptFunc;

    using InvokeStartAtScriptFunc = void (*)(ScriptHandle handle);
    extern InvokeStartAtScriptFunc invokeStartAtScriptFunc;

    using GetAvailableScripts = std::vector<const char *> (*)();
    extern GetAvailableScripts getAvailableScripts;
} // namespace ExternalCalls

//////////////////////////////////////////////////////////////////
///---------------------Functions binding----------------------///
//////////////////////////////////////////////////////////////////

using SymbolsLoadFunc = void *(*)(const char *name);

void *loadInternalCall(const char *name);
int loadExternalCalls(SymbolsLoadFunc load);

} // namespace Panda