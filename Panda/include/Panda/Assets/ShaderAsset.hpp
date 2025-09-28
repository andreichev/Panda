//
// Created by Andreichev Mikhail on 06.11.2024.
//

#pragma once

#include "Base/AssetHandler.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

/// Takes only compiled binaries as inputs
class ShaderAsset : public Asset {
public:
    ShaderAsset();
    ShaderAsset(const path_t &fragmentBinPath);
    ShaderAsset(const path_t &vertexBinPath, const path_t &fragmentBinPath);
    ShaderAsset(ShaderAsset &&other);
    ~ShaderAsset();
    Miren::ProgramHandle getMirenHandle();

protected:
    void create(const path_t &vertexBinPath, const path_t &fragmentBinPath);

private:
    Miren::ProgramHandle m_handle;
};

} // namespace Panda
