//
// Created by Michael Andreichev on 20.10.2024.
//

#pragma once

#include "Panda/Assets/AssetHandler.hpp"

namespace Panda {

class TextureAssetHandler : public AssetHandler {
public:
    Foundation::Shared<Asset> loadEditor(const Panda::path_t &filePath) override;
};

} // namespace Panda