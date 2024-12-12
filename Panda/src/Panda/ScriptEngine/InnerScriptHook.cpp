#include "Panda/ScriptEngine/InnerScriptHook.hpp"
#include "Panda/Application/Application.hpp"
#include "Panda/GameLogic/Input.hpp"
#include "Panda/GameLogic/GameContext.hpp"
#include "Panda/GameLogic/Components/SkyComponent.hpp"
#include "Rain/TypeName.hpp"

#include <unordered_map>

namespace Panda {

std::unordered_map<std::string, std::function<void(Entity &)>> s_createComponentFuncs;
std::unordered_map<std::string, std::function<bool(Entity &)>> s_hasComponentFuncs;
std::unordered_map<std::string, std::function<void(Entity &)>> s_removeComponentFuncs;

template<typename TComponent>
static void registerManagedComponent() {
    const std::string typeName = std::string(Rain::typeName<TComponent>());

    s_createComponentFuncs[typeName] = [](Entity &entity) { entity.addComponent<TComponent>(); };
    s_hasComponentFuncs[typeName] = [](Entity &entity) {
        return entity.hasComponent<TComponent>();
    };
    s_removeComponentFuncs[typeName] = [](Entity &entity) { entity.removeComponent<TComponent>(); };
}

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

    UUID world_FindByTag(const char *tag) {
        return GameContext::s_currentWorld->findByTag(tag).getId();
    }

    UUID world_CreateEntity(const char *tag) {
        Entity entity = GameContext::s_currentWorld->instantiateEntity();
        entity.setName(tag);
        return entity.getId();
    }

    void world_DestroyEntity(UUID id) {
        Entity entity = GameContext::s_currentWorld->getById(id);
        GameContext::s_currentWorld->destroy(entity);
    }

    /// ENTITY

    void entity_CreateComponent(UUID entityId, const char *type) {
        Entity entity = GameContext::s_currentWorld->getById(entityId);
        PND_ASSERT(type != nullptr, "TYPE IS NULL");
        if (s_createComponentFuncs.find(type) == s_createComponentFuncs.end()) {
            PND_ASSERT_F(false, "UNKNOWN TYPE {}", type);
            return;
        }
        s_createComponentFuncs.at(type)(entity);
    }

    bool entity_HasComponent(UUID entityId, const char *type) {
        Entity entity = GameContext::s_currentWorld->getById(entityId);
        PND_ASSERT(type != nullptr, "TYPE IS NULL");
        if (s_hasComponentFuncs.find(type) == s_hasComponentFuncs.end()) {
            PND_ASSERT_F(false, "UNKNOWN TYPE {}", type);
            return false;
        }
        return s_hasComponentFuncs.at(type)(entity);
    }

    void entity_RemoveComponent(UUID entityId, const char *type) {
        Entity entity = GameContext::s_currentWorld->getById(entityId);
        PND_ASSERT(type != nullptr, "TYPE IS NULL");
        if (s_removeComponentFuncs.find(type) == s_removeComponentFuncs.end()) {
            PND_ASSERT_F(false, "UNKNOWN TYPE {}", type);
            return;
        }
        if (!entity_HasComponent(entityId, type)) {
            PND_ASSERT_F(false, "ENTITY {} DOES NOT HAVE COMPONENT {}", (uint32_t)entityId, type);
            return;
        }
        s_removeComponentFuncs.at(type)(entity);
    }

    /// TRANSFORM COMPONENT

    void transformComponent_GetPosition(UUID entityId, float *x, float *y, float *z) {
        Entity entity = GameContext::s_currentWorld->getById(entityId);
        TransformComponent &transformComponent = entity.getTransform();
        glm::vec3 position = transformComponent.getPosition();
        *x = position.x;
        *y = position.y;
        *z = position.z;
    }

    void transformComponent_SetPosition(UUID entityId, float x, float y, float z) {
        Entity entity = GameContext::s_currentWorld->getById(entityId);
        TransformComponent &transformComponent = entity.getTransform();
        transformComponent.setPosition({x, y, z});
    }

    /// CONSOLE
    void console_Log(const char *message) {
        LOG_EDITOR(message);
    }
} // namespace InternalCalls

std::unordered_map<std::string, void *> g_scriptSymbols;

void initScriptHook() {
    /// APPLICATION
    g_scriptSymbols["application_Quit"] = (void *)InternalCalls::application_Quit;
    g_scriptSymbols["application_GetWidth"] = (void *)InternalCalls::application_GetWidth;
    g_scriptSymbols["application_GetHeight"] = (void *)InternalCalls::application_GetHeight;
    /// INPUT
    g_scriptSymbols["input_IsKeyPressed"] = (void *)InternalCalls::input_IsKeyPressed;
    g_scriptSymbols["input_IsKeyJustPressed"] = (void *)InternalCalls::input_IsKeyJustPressed;
    /// WORLD
    g_scriptSymbols["world_Load"] = (void *)InternalCalls::world_Load;
    g_scriptSymbols["world_FindByTag"] = (void *)InternalCalls::world_FindByTag;
    g_scriptSymbols["world_CreateEntity"] = (void *)InternalCalls::world_CreateEntity;
    g_scriptSymbols["world_DestroyEntity"] = (void *)InternalCalls::world_DestroyEntity;
    /// ENTITY
    g_scriptSymbols["entity_CreateComponent"] = (void *)InternalCalls::entity_CreateComponent;
    g_scriptSymbols["entity_HasComponent"] = (void *)InternalCalls::entity_HasComponent;
    g_scriptSymbols["entity_RemoveComponent"] = (void *)InternalCalls::entity_RemoveComponent;
    /// TRANSFORM COMPONENT
    g_scriptSymbols["transformComponent_GetPosition"] =
        (void *)InternalCalls::transformComponent_GetPosition;
    g_scriptSymbols["transformComponent_SetPosition"] =
        (void *)InternalCalls::transformComponent_SetPosition;
    /// CONSOLE
    g_scriptSymbols["console_Log"] = (void *)InternalCalls::console_Log;

    registerManagedComponent<SpriteRendererComponent>();
    registerManagedComponent<CameraComponent>();
    registerManagedComponent<Rigidbody2DComponent>();
    registerManagedComponent<BoxCollider2DComponent>();
    registerManagedComponent<SkyComponent>();
}

namespace ExternalCalls {
    Clear clear = nullptr;
    InstantiateScript instantiateScript = nullptr;
    SetFieldValue setFieldValue = nullptr;
    InvokeUpdateAtScript invokeUpdateAtScript = nullptr;
    InvokeStartAtScript invokeStartAtScript = nullptr;
    GetManifest getManifest = nullptr;
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
    clear = (Clear)load("clear");
    instantiateScript = (InstantiateScript)load("instantiateScript");
    setFieldValue = (SetFieldValue)load("setFieldValue");
    invokeUpdateAtScript = (InvokeUpdateAtScript)load("invokeUpdateAtScript");
    invokeStartAtScript = (InvokeStartAtScript)load("invokeStartAtScript");
    getManifest = (GetManifest)load("getManifest");
    LOG_INFO("SCRIPT ENGINE: Inner functions binding done.");
    return 0;
}

} // namespace Panda