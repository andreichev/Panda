//
// Created by Andreichev Mikhail on 06.11.2024.
//

#pragma once

#include "Panda/Assets/AssetHandler.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

class GpuProgram : public Asset {
public:
    GpuProgram();
    /// Editor constructor
    GpuProgram(const path_t &vertexPath, const path_t &fragmentPath);
    /// Runtime constructor
    GpuProgram(Foundation::Memory vertexData, Foundation::Memory fragmentData);

private:
    Miren::ProgramHandle m_handle;
};

} // namespace Panda
