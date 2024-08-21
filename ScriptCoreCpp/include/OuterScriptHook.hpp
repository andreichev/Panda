#pragma once

#include <stdint.h>

namespace Panda {

using ScriptHandle = uint32_t;

namespace InternalCalls {}

namespace ExternalCalls {
    using ApplicationQuit = void (*)();
    extern ApplicationQuit applicationQuit;

    using AddScript = void (*)(int script);
    extern AddScript addScript;
} // namespace ExternalCalls

}; // namespace Panda
