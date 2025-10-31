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
        , m_shaderRef()
        , m_data() {}

    MaterialAsset(const MaterialData &data, AssetRef<ShaderAsset> shader)
        : Asset(AssetType::MATERIAL)
        , m_shaderRef(shader)
        , m_data(data) {}

    AssetRef<ShaderAsset> getShaderAsset() {
        return m_shaderRef;
    }

    bool isValid() {
        return m_shaderRef->getMirenHandle().isValid();
    }

    void bindFields();

    void setFieldValue(const char *name, MaterialFieldValue value);

#ifdef PND_EDITOR
    const MaterialData &getInputs();
    void updateFields();
#endif

private:
    MaterialData m_data;
    AssetRef<ShaderAsset> m_shaderRef;
};

} // namespace Panda