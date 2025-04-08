#pragma once

#include "Asset.hpp"
#include "Panda/Assets/Path.hpp"

#include <Miren/Base.hpp>
#include <string>

namespace Panda {

class AssetHandler {
public:
    virtual ~AssetHandler() = default;
    virtual Foundation::Shared<Asset> load(AssetId id) = 0;
};

} // namespace Panda
