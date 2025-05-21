#include "Panda/Assets/GpuProgramAsset.hpp"
#include "Panda/Assets/Base/AssetImporterBase.hpp"

namespace Panda {

GpuProgramAsset::GpuProgramAsset(const path_t &vertexCodePath, const path_t &fragmentCodePath) {
    Foundation::Memory vertexMem = Panda::AssetImporterBase::loadData(vertexCodePath);
    Foundation::Memory fragmentMem = Panda::AssetImporterBase::loadData(fragmentCodePath);
    m_handle = Miren::createProgram({vertexMem, fragmentMem});
}

} // namespace Panda