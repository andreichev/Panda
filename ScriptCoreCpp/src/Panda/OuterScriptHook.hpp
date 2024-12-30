#pragma once

#include "Panda/Base.hpp"

namespace Panda {

namespace ExternalCalls {

    /// APPLICATION
    using Application_Quit = void (*)();
    extern Application_Quit application_Quit;
    using Application_GetWidth = uint32_t (*)();
    extern Application_GetWidth application_GetWidth;
    using Application_GetHeight = uint32_t (*)();
    extern Application_GetHeight application_GetHeight;
    /// WORLD
    using World_Load = void (*)(const char *name);
    extern World_Load world_Load;
    using World_FindByTag = EntityHandle (*)(const char *tag);
    extern World_FindByTag world_FindByTag;
    using World_CreateEntity = EntityHandle (*)(const char *tag);
    extern World_CreateEntity world_CreateEntity;
    using World_DestroyEntity = void (*)(EntityHandle id);
    extern World_DestroyEntity world_DestroyEntity;
    /// INPUT
    using Input_IsKeyPressed = bool (*)(int key);
    extern Input_IsKeyPressed input_IsKeyPressed;
    using Input_IsKeyJustPressed = bool (*)(int key);
    extern Input_IsKeyJustPressed input_IsKeyJustPressed;
    /// ENTITY
    using Entity_CreateComponent = void (*)(EntityHandle entityId, const char *type);
    extern Entity_CreateComponent entity_CreateComponent;
    using Entity_HasComponent = bool (*)(EntityHandle entityId, const char *type);
    extern Entity_HasComponent entity_HasComponent;
    using Entity_RemoveComponent = void (*)(EntityHandle entityId, const char *type);
    extern Entity_RemoveComponent entity_RemoveComponent;
    using Entity_GetName = const char *(*)(EntityHandle entityId);
    extern Entity_GetName entity_GetName;
    /// TRANSFORM COMPONENT
    using TransformComponent_GetPosition =
        void (*)(EntityHandle entityId, float *x, float *y, float *z);
    extern TransformComponent_GetPosition transformComponent_GetPosition;
    using TransformComponent_SetPosition =
        void (*)(EntityHandle entityId, float x, float y, float z);
    extern TransformComponent_SetPosition transformComponent_SetPosition;
    using TransformComponent_GetRotationEuler =
        void (*)(EntityHandle entityId, float *x, float *y, float *z);
    extern TransformComponent_GetRotationEuler transformComponent_GetRotationEuler;
    using TransformComponent_SetRotationEuler =
        void (*)(EntityHandle entityId, float x, float y, float z);
    extern TransformComponent_SetRotationEuler transformComponent_SetRotationEuler;
    using TransformComponent_GetScale =
        void (*)(EntityHandle entityId, float *x, float *y, float *z);
    extern TransformComponent_GetScale transformComponent_GetScale;
    using TransformComponent_SetScale = void (*)(EntityHandle entityId, float x, float y, float z);
    extern TransformComponent_SetScale transformComponent_SetScale;
    /// SPRITE RENDERER COMPONENT
    using SpriteRendererComponent_GetColor =
        void (*)(EntityHandle entityId, float *r, float *g, float *b, float *a);
    extern SpriteRendererComponent_GetColor spriteRendererComponent_GetColor;
    using SpriteRendererComponent_SetColor =
        void (*)(EntityHandle entityId, float r, float g, float b, float a);
    extern SpriteRendererComponent_SetColor spriteRendererComponent_SetColor;
    using SpriteRendererComponent_SetTexture =
        void (*)(EntityHandle entityId, TextureHandle textureId);
    extern SpriteRendererComponent_SetTexture spriteRendererComponent_SetTexture;
    using SpriteRendererComponent_SetCell =
        void (*)(EntityHandle entityId, int cols, int rows, int index);
    extern SpriteRendererComponent_SetCell spriteRendererComponent_SetCell;
    /// RIGIDBODY2D COMPONENT
    using Rigidbody2DComponent_applyForce = void (*)(EntityHandle entityId, float x, float y);
    extern Rigidbody2DComponent_applyForce rigidbody2DComponent_applyForce;
    using Rigidbody2DComponent_applyLinearImpulse =
        void (*)(EntityHandle entityId, float x, float y);
    extern Rigidbody2DComponent_applyLinearImpulse rigidbody2DComponent_applyLinearImpulse;
    using Rigidbody2DComponent_getLinearVelocity =
        void (*)(EntityHandle entityId, float *x, float *y);
    extern Rigidbody2DComponent_getLinearVelocity rigidbody2DComponent_getLinearVelocity;
    using Rigidbody2DComponent_setLinearVelocity =
        void (*)(EntityHandle entityId, float x, float y);
    extern Rigidbody2DComponent_setLinearVelocity rigidbody2DComponent_setLinearVelocity;
    using Rigidbody2DComponent_getMass = void (*)(EntityHandle entityId, float *mass);
    extern Rigidbody2DComponent_getMass rigidbody2DComponent_getMass;
    using Rigidbody2DComponent_getFriction = void (*)(EntityHandle entityId, float *friction);
    extern Rigidbody2DComponent_getFriction rigidbody2DComponent_getFriction;
    using Rigidbody2DComponent_setFriction = void (*)(EntityHandle entityId, float friction);
    extern Rigidbody2DComponent_setFriction rigidbody2DComponent_setFriction;
    /// LOG
    using Console_Log = void (*)(int type, const char *);
    extern Console_Log console_Log;

} // namespace ExternalCalls

}; // namespace Panda
