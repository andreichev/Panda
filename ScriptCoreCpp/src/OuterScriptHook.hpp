#pragma once

#include <stdint.h>

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
