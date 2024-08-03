//
// Created by Michael Andreichev on 02.08.2024.
//

#include "PandaScriptHook.hpp"

namespace PandaScriptHook {

ApplicationQuit applicationQuit = nullptr;

LIB_EXPORT int scriptSymbolsBindWithLoader(ScriptsLoadFunc load) {
    applicationQuit = (ApplicationQuit)load("ApplicationQuit");
    return 0;
}

} // namespace PandaScriptHook