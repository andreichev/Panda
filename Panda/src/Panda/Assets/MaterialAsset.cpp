#include "Panda/Assets/MaterialAsset.hpp"

namespace Panda {

void MaterialAsset::bindFields() {
    Miren::ProgramHandle shaderHandle = m_shaderRef->getMirenHandle();
    if (!shaderHandle.isValid()) { return; }
    int textureSlot = 0;
    for (auto &field : m_fields) {
        switch (field.type) {
            case MaterialFieldType::INTEGER: {
                float value = std::get<int>(field.value);
                Miren::setUniform(shaderHandle, field.name.c_str(), &value, Miren::Float);
                break;
            }
            case MaterialFieldType::FLOAT: {
                float value = std::get<float>(field.value);
                Miren::setUniform(shaderHandle, field.name.c_str(), &value, Miren::Float);
                break;
            }
            case MaterialFieldType::TEXTURE: {
                auto asset = std::get<AssetRef<Asset>>(field.value);
                auto texture = asset.as<TextureAsset>();
                Miren::setTexture(texture->getMirenHandle(), textureSlot);
                Miren::setUniform(
                    shaderHandle, field.name.c_str(), &textureSlot, Miren::UniformType::Sampler
                );
                textureSlot++;
                break;
            }
            default: {
                PND_ASSERT(false, "Unknown field type");
                break;
            }
        }
    }
}

} // namespace Panda