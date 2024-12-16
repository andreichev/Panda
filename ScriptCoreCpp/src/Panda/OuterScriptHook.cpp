//
// Created by Michael Andreichev on 02.08.2024.
//

#include "Panda/ScriptRegistry.hpp"
#include "Panda/OuterScriptHook.hpp"
#include "Panda/ScriptClassMaper.hpp"
#include "Panda/Manifest/ScriptBundleManifest.hpp"

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
    /// ENTITY
    Entity_CreateComponent entity_CreateComponent = nullptr;
    Entity_HasComponent entity_HasComponent = nullptr;
    Entity_RemoveComponent entity_RemoveComponent = nullptr;
    /// TRANSFORM COMPONENT
    TransformComponent_GetPosition transformComponent_GetPosition = nullptr;
    TransformComponent_SetPosition transformComponent_SetPosition = nullptr;
    /// RIGIDBODY2D COMPONENT
    Rigidbody2DComponent_applyForce rigidbody2DComponent_applyForce = nullptr;
    Rigidbody2DComponent_applyLinearImpulse rigidbody2DComponent_applyLinearImpulse = nullptr;
    Rigidbody2DComponent_getLinearVelocity rigidbody2DComponent_getLinearVelocity = nullptr;
    Rigidbody2DComponent_setLinearVelocity rigidbody2DComponent_setLinearVelocity = nullptr;
    Rigidbody2DComponent_getMass rigidbody2DComponent_getMass = nullptr;
    /// CONSOLE
    Console_Log console_Log = nullptr;
} // namespace ExternalCalls

namespace InternalCalls {
    void clear() {
        getScriptRegistry()->clear();
    }

    ScriptInstanceHandle instantiateScript(EntityHandle entityId, const char *name) {
        return getScriptRegistry()->instantiate(Bamboo::Entity(entityId), name);
    }

    void invokeUpdateAtScript(ScriptInstanceHandle handle, float deltaTime) {
        Bamboo::Script *script = getScriptRegistry()->getInstanceWithId(handle);
        if (!script) {
            // assert(false);
            return;
        }
        script->update(deltaTime);
    }

    void invokeStartAtScript(ScriptInstanceHandle handle) {
        Bamboo::Script *script = getScriptRegistry()->getInstanceWithId(handle);
        if (!script) {
            // assert(false);
            return;
        }
        script->start();
    }

    void setFieldValue(ScriptInstanceHandle scriptId, FieldHandle fieldId, void *value) {
        getScriptRegistry()->setFieldValue(scriptId, fieldId, value);
    }

    ScriptBundleManifest getManifest() {
        return ScriptClassMapper::getClassesManifest(getScriptRegistry()->m_scriptClasses);
    }
} // namespace InternalCalls

std::unordered_map<std::string, void *> g_scriptSymbols;

void initScriptHook() {
    using namespace InternalCalls;
    g_scriptSymbols["clear"] = (void *)clear;
    g_scriptSymbols["instantiateScript"] = (void *)instantiateScript;
    g_scriptSymbols["setFieldValue"] = (void *)setFieldValue;
    g_scriptSymbols["invokeStartAtScript"] = (void *)invokeStartAtScript;
    g_scriptSymbols["invokeUpdateAtScript"] = (void *)invokeUpdateAtScript;
    g_scriptSymbols["getManifest"] = (void *)getManifest;
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
    /// APPLICATION
    application_Quit = (Application_Quit)load("application_Quit");
    application_GetWidth = (Application_GetWidth)load("application_GetWidth");
    application_GetHeight = (Application_GetHeight)load("application_GetHeight");
    /// WORLD
    world_Load = (World_Load)load("world_Load");
    world_FindByTag = (World_FindByTag)load("world_FindByTag");
    world_CreateEntity = (World_CreateEntity)load("world_CreateEntity");
    world_DestroyEntity = (World_DestroyEntity)load("world_DestroyEntity");
    /// INPUT
    input_IsKeyPressed = (Input_IsKeyPressed)load("input_IsKeyPressed");
    input_IsKeyJustPressed = (Input_IsKeyJustPressed)load("input_IsKeyJustPressed");
    /// ENTITY
    entity_CreateComponent = (Entity_CreateComponent)load("entity_CreateComponent");
    entity_HasComponent = (Entity_HasComponent)load("entity_HasComponent");
    entity_RemoveComponent = (Entity_RemoveComponent)load("entity_RemoveComponent");
    /// TRANSFORM COMPONENT
    transformComponent_GetPosition =
        (TransformComponent_GetPosition)load("transformComponent_GetPosition");
    transformComponent_SetPosition =
        (TransformComponent_SetPosition)load("transformComponent_SetPosition");
    /// RIGIDBODY2D COMPONENT
    rigidbody2DComponent_applyForce =
        (Rigidbody2DComponent_applyForce)load("rigidbody2DComponent_applyForce");
    rigidbody2DComponent_applyLinearImpulse =
        (Rigidbody2DComponent_applyLinearImpulse)load("rigidbody2DComponent_applyLinearImpulse");
    rigidbody2DComponent_getLinearVelocity =
        (Rigidbody2DComponent_getLinearVelocity)load("rigidbody2DComponent_getLinearVelocity");
    rigidbody2DComponent_setLinearVelocity =
        (Rigidbody2DComponent_setLinearVelocity)load("rigidbody2DComponent_setLinearVelocity");
    rigidbody2DComponent_getMass =
        (Rigidbody2DComponent_getMass)load("rigidbody2DComponent_getMass");
    /// LOG
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