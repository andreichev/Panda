#pragma once

#include <Miren/Miren.hpp>

namespace Panda {

struct TextureBinding {
    const char *name;
    Miren::TextureHandle texture;
};

} // namespace Panda