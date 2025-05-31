#pragma once

#include <Miren/Miren.hpp>
#include <array>

#include "Path.hpp"

namespace Panda {

class AssetImporterBase final {
public:
    static Miren::TextureCreate load2DTexture(const path_t &path);
    static Miren::TextureCreate loadCubeMapTexture(std::array<path_t, 6> paths);
    static Foundation::Memory loadData(const path_t &path);
};

} // namespace Panda
