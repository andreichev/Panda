#pragma once

#include <stdint.h>

namespace Panda {

using ScriptHandle = uint32_t;

namespace ExternalCalls {
    using ApplicationQuit = void (*)();
    extern ApplicationQuit applicationQuit;

    using ConsoleLog = void (*)(const char *);
    extern ConsoleLog consoleLog;
} // namespace ExternalCalls

}; // namespace Panda
