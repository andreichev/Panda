#pragma once

#include "Panda/Assets/Base/AssetRef.hpp"
#include "Panda/Serialization/AssetsDtos.hpp"

namespace Panda {

using MaterialFieldData = std::variant<int, float, Vec4, AssetRef<Asset>>;

struct MaterialField {
    std::string name;
    MaterialFieldType type;
    MaterialFieldData value;
};

struct MaterialData {
    std::vector<MaterialField> inputs;
};

} // namespace Panda