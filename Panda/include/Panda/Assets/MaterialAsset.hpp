#pragma once

#include "Panda/Base/Base.hpp"
#include "Panda/GameLogic/GameContext.hpp"
#include "Panda/Assets/TextureAsset.hpp"
#include "Panda/Renderer/MaterialData.hpp"
#include "Base/AssetRef.hpp"
#include "ShaderAsset.hpp"

#include <Miren/Miren.hpp>
#include <cstdlib>

namespace Panda {

class MaterialAsset final : public Asset {
public:
    MaterialAsset()
        : Asset(AssetType::MATERIAL)
        , m_shaderRef() {}

    MaterialAsset(const MaterialData &data, AssetRef<ShaderAsset> shader)
        : Asset(AssetType::MATERIAL)
        , m_shaderRef(shader)
        , m_fields(data.inputs) {}

    AssetRef<ShaderAsset> getShaderAsset() {
        return m_shaderRef;
    }

    const std::vector<MaterialField> &getFields() {
        return m_fields;
    }

    bool isValid() {
        return m_shaderRef->getMirenHandle().isValid();
    }

    void bindFields();

    void setFieldValue(const char *name, MaterialFieldData value);

private:
    std::vector<MaterialField> m_fields;
    AssetRef<ShaderAsset> m_shaderRef;
};

} // namespace Panda