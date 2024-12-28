#pragma once

#include "Bamboo/Base.hpp"

namespace Bamboo {

class Texture final {
public:
    Texture();
    Texture(TextureHandle id);

    inline bool isValid() {
        return m_id != 0;
    }

    inline uint32_t getId() {
        return m_id;
    }

private:
    TextureHandle m_id;
};

} // namespace Bamboo