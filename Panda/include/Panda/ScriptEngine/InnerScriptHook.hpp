#pragma once

#include "Manifest/ScriptBundleManifest.hpp"

#include <stdint.h>
#include <Rain/UUID.hpp>

namespace Panda {

using ScriptInstanceHandle = uint32_t;

namespace ExternalCalls {
    using Clear = void (*)();
    extern Clear clear;

    using InstantiateScript = ScriptInstanceHandle (*)(UUID entityId, const char *name);
    extern InstantiateScript instantiateScript;

    using SetFieldValue = void (*)(ScriptInstanceHandle scriptId, FieldHandle fieldId, void *value);
    extern SetFieldValue setFieldValue;

    using InvokeUpdateAtScript = void (*)(ScriptInstanceHandle handle, float deltaTime);
    extern InvokeUpdateAtScript invokeUpdateAtScript;

    using InvokeStartAtScript = void (*)(ScriptInstanceHandle handle);
    extern InvokeStartAtScript invokeStartAtScript;

    using GetManifest = ScriptBundleManifest (*)();
    extern GetManifest getManifest;
} // namespace ExternalCalls

//////////////////////////////////////////////////////////////////
///---------------------Functions binding----------------------///
//////////////////////////////////////////////////////////////////

using SymbolsLoadFunc = void *(*)(const char *name);

void *loadInternalCall(const char *name);
int loadExternalCalls(SymbolsLoadFunc load);

} // namespace Panda