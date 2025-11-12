#pragma once

#include "Panda/Serialization/AssetsDtos.hpp"
#include "Panda/Renderer/MaterialData.hpp"

namespace Panda {

class AssetsMapper final {
public:
    static void toData(MaterialData &data, const MaterialDataDto &dto);
    static void toDto(const MaterialData &data, MaterialDataDto &dto);
};

} // namespace Panda