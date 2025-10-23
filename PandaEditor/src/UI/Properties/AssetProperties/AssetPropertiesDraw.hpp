//
// Created by Andreichev Mikhail on 27.07.2025.
//

#pragma once

#include <unordered_set>

#include <Panda/Assets/Base/Asset.hpp>
#include <Panda/Assets/Base/Path.hpp>

namespace Panda {

class AssetPropertiesDrawOutput {
public:
    virtual ~AssetPropertiesDrawOutput() = default;
    virtual void locateMissingAsset(AssetId id) = 0;
};

class AssetPropertiesDraw final {
public:
    AssetPropertiesDraw(AssetPropertiesDrawOutput *output);
    void drawProperties(const path_t &assetPath);
    void drawProperties(AssetId assetId);

private:
    AssetPropertiesDrawOutput *m_output;
};

} // namespace Panda
