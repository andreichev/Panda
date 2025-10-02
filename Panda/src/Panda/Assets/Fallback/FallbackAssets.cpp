#include "Panda/Assets/Fallback/FallbackAssets.hpp"

namespace Panda {

FallbackTextureAsset::FallbackTextureAsset() {
    Foundation::Memory violetTextureData = Foundation::Memory::alloc(sizeof(uint32_t));
    *(uint32_t *)violetTextureData.data = 0xFFFF008F;
    Miren::TextureCreate violetTextureCreate;
    violetTextureCreate.m_data = violetTextureData;
    create(violetTextureCreate);
}

} // namespace Panda