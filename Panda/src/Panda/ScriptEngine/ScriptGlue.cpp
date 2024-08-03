#include "ScriptGlue.hpp"
#include "Panda.hpp"

#include <unordered_map>

namespace Panda {

void applicationQuit() {
    Panda::Application::get()->close();
}

std::unordered_map<std::string, void *> g_scriptSymbols;

void initScriptGlue() {
    g_scriptSymbols["applicationQuit"] = (void *)applicationQuit;
}

void *loadScriptFunc(const char *name) {
    if (g_scriptSymbols.empty()) {
        initScriptGlue();
    }
    if (g_scriptSymbols.find(name) == g_scriptSymbols.end()) {
        return nullptr;
    }
    return g_scriptSymbols.at(name);
}

} // namespace Panda