#include "Panda/Assets/MaterialAsset.hpp"
#include "Panda/Renderer/Std140Buffer.hpp"
#include "Panda/Serialization/ShaderReflectionDataTransformer.hpp"

namespace Panda {

void MaterialAsset::bindFields() {
    Miren::ProgramHandle shaderHandle = m_shaderRef->getMirenHandle();
    if (!shaderHandle.isValid()) { return; }
    Std140Buffer ubo;
    for (auto &field : m_data.inputs) {
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
                UUID assetId = std::get<UUID>(field.value);
                if (!assetId) {
                    break;
                }
                auto handler = GameContext::getAssetHandler();
                AssetRef<TextureAsset> texture = handler->makeRef<TextureAsset>(assetId);
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
        Miren::addInputUniformBuffer(
            shaderHandle, "type_MATERIAL_FIELDS", ubo.getData(), ubo.getSize()
        );
    }
}

void MaterialAsset::setFieldValue(const char *name, MaterialFieldValue value) {
    for (auto &field : m_data.inputs) {
        if (field.name == name) {
            field.value = value;
            return;
        }
    }
    LOG_ERROR_EDITOR("MATERIAL FIELD %s NOT FOUND", name);
}

#ifdef PND_EDITOR

MaterialData &MaterialAsset::getInputs() {
    return m_data;
}

void MaterialAsset::updateFields() {
    ShaderSpirvReflectionData reflection = m_shaderRef->getReflectionData();
    MaterialData shaderData = ShaderReflectionDataTransformer::transformToMaterialData(reflection);
    for (auto &field : m_data.inputs) {
        const MaterialField &shaderField = shaderData.getField(field.name);
        if (!shaderField) {
            LOG_INFO_EDITOR("MATERIAL FIELD %s NOT FOUND.", field.name.c_str());
            m_data.removeField(field);
            continue;
        }
    }
    for (const auto &shaderField : shaderData.inputs) {
        // Check if field is new
        if (!m_data.hasField(shaderField.name)) {
            MaterialField field =
                MaterialField(shaderField.name, shaderField.type, shaderField.value);
            m_data.addField(field);
            continue;
        }
        MaterialField &field = m_data.getField(shaderField.name);
        if (field.type != shaderField.type) {
            field.type = shaderField.type;
            field.value = shaderField.value;
        }
    }
}

void MaterialAsset::setChanged(bool value) {
    m_isChanged = value;
}

bool MaterialAsset::isChanged() {
    return m_isChanged;
}

#endif

} // namespace Panda