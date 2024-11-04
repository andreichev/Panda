#pragma once

#include <Panda/Base/FileBase.hpp>
#include <Panda/Assets/Asset.hpp>

namespace Panda {

struct AssetInfoEditor {
    AssetId id;
    AssetType type;
    path_t path;
};

} // namespace Panda