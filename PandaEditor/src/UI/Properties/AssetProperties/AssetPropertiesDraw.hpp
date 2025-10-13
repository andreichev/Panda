//
// Created by Andreichev Mikhail on 27.07.2025.
//

#include <unordered_set>

#include <Panda/Assets/Base/Asset.hpp>
#include <Panda/Assets/Base/Path.hpp>

namespace Panda {

class AssetPropertiesDraw final {
public:
    static void drawProperties(const path_t &assetPath);
    static void drawProperties(AssetId assetId);
};

} // namespace Panda
