#pragma once

#include <cstdint>

namespace Panda {

class Application final {
public:
    static uint32_t getWidth();
    static uint32_t getHeight();
    static void loadWorld(const char *name);
    static void quit();
};

} // namespace Panda