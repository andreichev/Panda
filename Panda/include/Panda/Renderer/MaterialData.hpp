#pragma once

#include "Panda/Serialization/AssetsDtos.hpp"
#include "Panda/Assets/TextureAsset.hpp"

namespace Panda {

using MaterialFieldData = std::variant<int, float, Foundation::Shared<Asset>>;

struct MaterialField {
    std::string name;
    MaterialFieldType type;
    MaterialFieldData value;
};

struct MaterialData {
    std::vector<MaterialField> inputs;
};

} // namespace Panda