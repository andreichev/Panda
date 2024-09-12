#pragma once

#include <stdint.h>

namespace Panda {

namespace ExternalCalls {
    /// APPLICATION
    using ApplicationQuit = void (*)();
    extern ApplicationQuit applicationQuit;
    using ApplicationGetWidth = uint32_t (*)();
    extern ApplicationGetWidth applicationGetWidth;
    using ApplicationGetHeight = uint32_t (*)();
    extern ApplicationGetHeight applicationGetHeight;
    using ConsoleLog = void (*)(const char *);
    /// LOG
    extern ConsoleLog consoleLog;
} // namespace ExternalCalls

}; // namespace Panda
