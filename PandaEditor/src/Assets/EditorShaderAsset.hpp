//
// Created by Andreichev Mikhail on 26.09.2025.
//

#pragma once

#include <Panda/Assets/ShaderAsset.hpp>

namespace Panda {

/// Takes shaders source code as inputs
class EditorShaderAsset final : public ShaderAsset {
public:
    EditorShaderAsset(const path_t &vertexCodePath, const path_t &fragmentCodePath);

private:
    bool isInputNewer(const path_t &inputPath, const path_t &outputPath);
};

} // namespace Panda
