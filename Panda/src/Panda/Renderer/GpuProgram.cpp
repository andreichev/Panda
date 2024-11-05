#include "Panda/Renderer/GpuProgram.hpp"
#include "Panda/Assets/AssetLoaderEditor.hpp"

namespace Panda {

GpuProgram::GpuProgram()
    : m_handle(MIREN_INVALID_HANDLE) {}

GpuProgram::GpuProgram(const path_t &vertexPath, const path_t &fragmentPath) {
    Panda::ProgramData programAsset = Panda::AssetLoaderEditor::loadProgram(
        "editor-shaders/grid_vertex.glsl", "editor-shaders/grid_fragment.glsl"
    );
    m_handle = Miren::createProgram(programAsset.getMirenProgramCreate());
}

GpuProgram::GpuProgram(Foundation::Memory vertexData, Foundation::Memory fragmentData) {
    m_handle = Miren::createProgram({vertexData, fragmentData});
}

} // namespace Panda