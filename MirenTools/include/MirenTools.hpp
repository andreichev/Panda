#pragma once

#include <vector>
#include <filesystem>

using path_t = std::filesystem::path;

namespace MirenTools {

enum ShaderType { VERTEX, FRAGMENT };

/// For use with Vulkan. Recommended
void compileHlslShaderToSpv(const path_t &input, const path_t &output, ShaderType shaderType);
/// Converting SpirV to OpenGL shader language for old v 330
void compileSpvShaderToGlsl(const path_t &input, const path_t &output, ShaderType shaderType);

} // namespace MirenTools