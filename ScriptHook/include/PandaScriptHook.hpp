#pragma once

#if defined(_WIN32) || defined(_WIN64)
#    define LIB_EXPORT __declspec(dllexport)
#else
#    define LIB_EXPORT
#endif

namespace PandaScriptHook {

using ScriptsLoadFunc = void *(*)(const char *name);

extern "C" {

int scriptSymbolsBindWithLoader(ScriptsLoadFunc load);
}

using ApplicationQuit = void (*)();
extern ApplicationQuit applicationQuit;

}; // namespace PandaScriptHook
