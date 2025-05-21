//
// Created by Andreichev Mikhail on 06.11.2024.
//

#pragma once

#include "Base/AssetHandler.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

class GpuProgramAsset : public Asset {
public:
    GpuProgramAsset(const path_t &vertexCodePath, const path_t &fragmentCodePath);

private:
    Miren::ProgramHandle m_handle;
};

} // namespace Panda
