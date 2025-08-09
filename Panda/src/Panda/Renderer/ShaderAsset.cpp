#include "Panda/Assets/ShaderAsset.hpp"
#include "Panda/Assets/Base/AssetImporterBase.hpp"

namespace Panda {

ShaderAsset::ShaderAsset()
    : m_handle(MIREN_INVALID_HANDLE) {}

ShaderAsset::ShaderAsset(const path_t &fragmentCodePath) {
    path_t vertexCodePath = "default-shaders/default_vertex.glsl";
    Foundation::Memory vertexMem = Panda::AssetImporterBase::loadData(vertexCodePath);
    Foundation::Memory fragmentMem = Panda::AssetImporterBase::loadData(fragmentCodePath);
    m_handle = Miren::createProgram({vertexMem, fragmentMem});
}

ShaderAsset::ShaderAsset(const path_t &vertexCodePath, const path_t &fragmentCodePath) {
    Foundation::Memory vertexMem = Panda::AssetImporterBase::loadData(vertexCodePath);
    Foundation::Memory fragmentMem = Panda::AssetImporterBase::loadData(fragmentCodePath);
    m_handle = Miren::createProgram({vertexMem, fragmentMem});
}

ShaderAsset::ShaderAsset(ShaderAsset &&other) {
    m_handle = other.m_handle;
    other.m_handle = MIREN_INVALID_HANDLE;
}

ShaderAsset::~ShaderAsset() {
    if (m_handle.isValid()) { Miren::deleteProgram(m_handle); }
}

Miren::ProgramHandle ShaderAsset::getMirenHandle() {
    return m_handle;
}

} // namespace Panda