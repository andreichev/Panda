#pragma once

#include <Miren/Miren.hpp>

namespace Panda {

struct TextureBinding {
    std::string name;
    Miren::TextureHandle texture;
};

} // namespace Panda