#pragma once

#include "Panda/Renderer/MaterialData.hpp"
#include "Panda/Renderer/ShaderSpirvReflectionData.hpp"

namespace Panda {

/// Transfers data from ShaderSpirvReflectionData to MaterialData
class ShaderReflectionDataTransformer final {
public:
    static MaterialData transformToMaterialData(const ShaderSpirvReflectionData &reflection);
};

} // namespace Panda