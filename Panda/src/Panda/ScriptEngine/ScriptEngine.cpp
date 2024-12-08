//
// Created by Michael Andreichev on 01.08.2024.
//

#include "Panda/ScriptEngine/ScriptEngine.hpp"
#include "Panda/ScriptEngine/InnerScriptHook.hpp"

#include <Foundation/Foundation.hpp>
#include <dylib.hpp>

namespace Panda {

ScriptEngine::ScriptEngine()
    : m_lib(nullptr)
    , m_manifest() {}

ScriptEngine::~ScriptEngine() {
    terminate();
}

void ScriptEngine::terminate() {
    if (m_lib) {
        F_DELETE(Foundation::getAllocator(), (dylib *)m_lib);
        m_lib = nullptr;
    }
}

ScriptBundleManifest ScriptEngine::getManifest() {
    return m_manifest;
}

bool ScriptEngine::reload(ScriptEngineConfig config) {
    if (m_lib) {
        ExternalCalls::clear();
    }
    terminate();
    m_manifest = {};
    m_lib = nullptr;
    try {
        dylib *pDylib = F_NEW(Foundation::getAllocator(), dylib)(
            config.dllPath.c_str(), config.dllName.c_str(), true
        );
        m_lib = pDylib;
        // ------------
        // OUTER LOADER
        // ------------
        auto outerExternalLoader =
            pDylib->get_function<int(Panda::SymbolsLoadFunc load)>("loadExternalCalls");
        outerExternalLoader(loadInternalCall);

        // ------------
        // INNER LOADER
        // ------------
        auto outerInternalLoader = pDylib->get_function<void *(const char *)>("loadInternalCall");
        loadExternalCalls(outerInternalLoader);
    } catch (...) {
        m_lib = nullptr;
        LOG_ERROR(
            "SCRIPT ENGINE: Can't load script dynamic library at path {}/{}",
            config.dllPath.string().c_str(),
            config.dllName.c_str()
        );
        return false;
    }
    m_manifest = ExternalCalls::getManifest();
    return true;
}

bool ScriptEngine::isLoaded() {
    return m_lib;
}

} // namespace Panda
