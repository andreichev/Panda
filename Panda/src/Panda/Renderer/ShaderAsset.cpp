#include "Panda/Assets/ShaderAsset.hpp"
#include "Panda/Assets/Base/AssetImporterBase.hpp"

namespace Panda {

ShaderAsset::ShaderAsset()
    : m_handle(MIREN_INVALID_HANDLE) {}

ShaderAsset::ShaderAsset(const path_t &fragmentBinPath) {
    path_t vertexBinPath = "default-shaders/default.vert";
    create(vertexBinPath, fragmentBinPath);
}

ShaderAsset::ShaderAsset(const path_t &vertexBinPath, const path_t &fragmentBinPath) {
    create(vertexBinPath, fragmentBinPath);
}

ShaderAsset::ShaderAsset(ShaderAsset &&other) {
    m_handle = other.m_handle;
    other.m_handle = MIREN_INVALID_HANDLE;
}

void ShaderAsset::create(const path_t &vertexBinPath, const path_t &fragmentBinPath) {
    Foundation::Memory vertexMem = Panda::AssetImporterBase::loadData(vertexBinPath);
    Foundation::Memory fragmentMem = Panda::AssetImporterBase::loadData(fragmentBinPath);
    m_handle = Miren::createProgram({vertexMem, fragmentMem});
}

ShaderAsset::~ShaderAsset() {
    if (m_handle.isValid()) { Miren::deleteProgram(m_handle); }
}

Miren::ProgramHandle ShaderAsset::getMirenHandle() {
    return m_handle;
}

} // namespace Panda