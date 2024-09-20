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
    using World_FindByTag = id_t (*)(const char *tag);
    extern World_FindByTag world_FindByTag;
    using World_CreateEntity = id_t (*)(const char *tag);
    extern World_CreateEntity world_CreateEntity;
    using World_DestroyEntity = void (*)(id_t id);
    extern World_DestroyEntity world_DestroyEntity;
    /// INPUT
    using Input_IsKeyPressed = bool (*)(int key);
    extern Input_IsKeyPressed input_IsKeyPressed;
    using Input_IsKeyJustPressed = bool (*)(int key);
    extern Input_IsKeyJustPressed input_IsKeyJustPressed;
    /// LOG
    using Console_Log = void (*)(const char *);
    extern Console_Log console_Log;
} // namespace ExternalCalls

}; // namespace Panda
