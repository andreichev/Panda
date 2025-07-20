//
// Created by Andreichev Mikhail on 06.11.2024.
//

#pragma once

#include "Base/AssetHandler.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

class ShaderAsset : public Asset {
public:
    ShaderAsset();
    ShaderAsset(const path_t &fragmentCodePath);
    ShaderAsset(const path_t &vertexCodePath, const path_t &fragmentCodePath);
    ShaderAsset(ShaderAsset &&other);
    ~ShaderAsset();
    Miren::ProgramHandle getMirenHandle();

private:
    Miren::ProgramHandle m_handle;
};

} // namespace Panda
