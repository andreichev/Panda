#pragma once

#include "Asset.hpp"
#include "Panda/Base/FileBase.hpp"

#include <Miren/Base.hpp>
#include <string>

namespace Panda {

class AssetHandler {
public:
    virtual ~AssetHandler() = default;
    virtual Foundation::Shared<Asset> load(AssetId id) = 0;

    static path_t s_defaultResourcesPath;
};

} // namespace Panda