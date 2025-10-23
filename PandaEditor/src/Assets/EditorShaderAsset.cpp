//
// Created by Andreichev Mikhail on 26.09.2025.
//

#include "EditorShaderAsset.hpp"

#include <MirenTools.hpp>

namespace Panda {

EditorShaderAsset::EditorShaderAsset(const path_t &vertexCodePath, const path_t &fragmentCodePath) {
    path_t vertexSpvPath = vertexCodePath;
    vertexSpvPath.replace_extension(".spv");
    if (isInputNewer(vertexCodePath, vertexSpvPath)) {
        MirenTools::compileHlslShaderToSpv(vertexCodePath, vertexSpvPath, MirenTools::VERTEX);
    }
    path_t fragmentSpvPath = fragmentCodePath;
    fragmentSpvPath.replace_extension(".spv");
    if (isInputNewer(fragmentCodePath, fragmentSpvPath)) {
        MirenTools::compileHlslShaderToSpv(fragmentCodePath, fragmentSpvPath, MirenTools::FRAGMENT);
    }
    /// TODO: Add check if renderer is OpenGL or Vulkan. If Vulkan, return here
    path_t vertexGlslPath = vertexCodePath;
    vertexGlslPath.replace_extension(".vert");
    if (isInputNewer(vertexSpvPath, vertexGlslPath)) {
        MirenTools::compileSpvShaderToGlsl(vertexSpvPath, vertexGlslPath, MirenTools::VERTEX);
    }
    path_t fragmentGlslPath = fragmentCodePath;
    fragmentGlslPath.replace_extension(".frag");
    if (isInputNewer(fragmentSpvPath, fragmentGlslPath)) {
        MirenTools::compileSpvShaderToGlsl(fragmentSpvPath, fragmentGlslPath, MirenTools::FRAGMENT);
    }
    create(vertexGlslPath, fragmentGlslPath);
}

bool EditorShaderAsset::isInputNewer(const path_t &inputPath, const path_t &outputPath) {
    if (!std::filesystem::exists(outputPath)) { return true; }
    auto inputTime = std::filesystem::last_write_time(inputPath);
    auto outputTime = std::filesystem::last_write_time(outputPath);
    return inputTime > outputTime;
}

} // namespace Panda