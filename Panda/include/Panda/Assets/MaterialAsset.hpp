#pragma once

#include "Panda/Base/Base.hpp"
#include "Panda/GameLogic/GameContext.hpp"
#include "Panda/Assets/TextureAsset.hpp"
#include "Base/Asset.hpp"
#include "Base/Path.hpp"
#include "ShaderAsset.hpp"

#include <Miren/Miren.hpp>
#include <cstdlib>

namespace Panda {

// TODO: Replace with material fields
struct TextureBinding {
    const char *name;
    Foundation::Shared<TextureAsset> texture;
};

class MaterialAsset final : public Asset {
public:
    MaterialAsset()
        : Asset(AssetType::MATERIAL) {}

    MaterialAsset(Foundation::Shared<ShaderAsset> shader, std::vector<TextureBinding> bindings)
        : Asset(AssetType::MATERIAL)
        , m_shader(shader)
        , m_bindings(bindings) {}

    Foundation::Shared<ShaderAsset> getShaderAsset() {
        return m_shader;
    }

    // TODO: Replace with material fields
    const std::vector<TextureBinding> &getBindings() {
        return m_bindings;
    }

    bool isValid() {
        return m_shader->getMirenHandle().isValid();
    }

private:
    std::vector<TextureBinding> m_bindings;
    Foundation::Shared<ShaderAsset> m_shader;
};

} // namespace Panda