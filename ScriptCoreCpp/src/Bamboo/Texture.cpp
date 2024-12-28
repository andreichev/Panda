#include "Bamboo/Texture.hpp"

namespace Bamboo {

Texture::Texture()
    : m_id(0) {}

Texture::Texture(TextureHandle id)
    : m_id(id) {}

} // namespace Bamboo