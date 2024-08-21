#pragma once

#include <stdint.h>

namespace Panda {

using ScriptHandle = uint32_t;

namespace ExternalCalls {
    using ApplicationQuit = void (*)();
    extern ApplicationQuit applicationQuit;
} // namespace ExternalCalls

}; // namespace Panda
