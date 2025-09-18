#include "Panda/Assets/MaterialAsset.hpp"
#include "Panda/Renderer/Std140Buffer.hpp"

namespace Panda {

void MaterialAsset::bindFields() {
    Miren::ProgramHandle shaderHandle = m_shaderRef->getMirenHandle();
    if (!shaderHandle.isValid()) { return; }
    Std140Buffer ubo;
    for (auto &field : m_fields) {
        switch (field.type) {
            case MaterialFieldType::FLOAT: {
                float value = std::get<float>(field.value);
                ubo.addFloat(value);
                break;
            }
            case MaterialFieldType::VEC4: {
                Vec4 value = std::get<Vec4>(field.value);
                ubo.addVec4(value.x, value.y, value.z, value.w);
                break;
            }
            case MaterialFieldType::TEXTURE: {
                auto asset = std::get<AssetRef<Asset>>(field.value);
                auto texture = asset.as<TextureAsset>();
                Miren::TextureHandle handle = texture->getMirenHandle();
                Miren::addInputTexture(shaderHandle, field.name.c_str(), handle);
                break;
            }
            default: {
                PND_ASSERT(false, "Unknown field type");
                break;
            }
        }
    }
    if (ubo.getSize()) {
        Miren::addInputUniformBuffer(shaderHandle, "MATERIAL_FIELDS", ubo.getData(), ubo.getSize());
    }
}

void MaterialAsset::setFieldValue(const char *name, MaterialFieldData value) {
    for (auto &field : m_fields) {
        if (field.name == name) {
            field.value = value;
            return;
        }
    }
    LOG_ERROR_EDITOR("MATERIAL FIELD %s NOT FOUND", name);
}

} // namespace Panda