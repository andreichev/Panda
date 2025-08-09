#pragma once

#include "Panda/Base/Base.hpp"
#include "Panda/GameLogic/GameContext.hpp"
#include "Panda/Assets/TextureAsset.hpp"
#include "Panda/Renderer/MaterialData.hpp"
#include "Base/Asset.hpp"
#include "Base/Path.hpp"
#include "ShaderAsset.hpp"

#include <Miren/Miren.hpp>
#include <cstdlib>

namespace Panda {

class MaterialAsset final : public Asset {
public:
    MaterialAsset()
        : Asset(AssetType::MATERIAL) {}

    MaterialAsset(const MaterialData &data, Foundation::Shared<ShaderAsset> shader)
        : Asset(AssetType::MATERIAL)
        , m_shader(shader)
        , m_fields(data.inputs) {}

    Foundation::Shared<ShaderAsset> getShaderAsset() {
        return m_shader;
    }

    const std::vector<MaterialField> &getFields() {
        return m_fields;
    }

    bool isValid() {
        return m_shader->getMirenHandle().isValid();
    }

    void bindFields();

private:
    std::vector<MaterialField> m_fields;
    Foundation::Shared<ShaderAsset> m_shader;
};

} // namespace Panda