//
// Created by Andreichev Mikhail on 26.09.2025.
//

#include "EditorShaderAsset.hpp"

#include <MirenTools.hpp>

namespace Panda {

EditorShaderAsset::EditorShaderAsset(const path_t &vertexCodePath, const path_t &fragmentCodePath) {
    path_t vertexSpvPath = vertexCodePath;
    vertexSpvPath.replace_extension(".spv");
    MirenTools::compileHlslShaderToSpv(vertexCodePath, vertexSpvPath, MirenTools::VERTEX);
    path_t fragmentSpvPath = fragmentCodePath;
    fragmentSpvPath.replace_extension(".spv");
    MirenTools::compileHlslShaderToSpv(fragmentCodePath, fragmentSpvPath, MirenTools::FRAGMENT);
    /// TODO: Add check if renderer is OpenGL or Vulkan. If Vulkan, return here
    path_t vertexGlslPath = vertexCodePath;
    vertexGlslPath.replace_extension(".vert");
    MirenTools::compileSpvShaderToGlsl(vertexSpvPath, vertexGlslPath, MirenTools::VERTEX);
    path_t fragmentGlslPath = vertexCodePath;
    fragmentGlslPath.replace_extension(".frag");
    MirenTools::compileSpvShaderToGlsl(fragmentSpvPath, fragmentGlslPath, MirenTools::FRAGMENT);
    create(vertexGlslPath, fragmentGlslPath);
}

} // namespace Panda