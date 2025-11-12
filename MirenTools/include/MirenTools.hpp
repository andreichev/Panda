#pragma once

#include <vector>
#include <filesystem>

using path_t = std::filesystem::path;

namespace MirenTools {

enum ShaderType { VERTEX, FRAGMENT };

/// For use with Vulkan. Recommended
bool compileHlslShaderToSpv(const path_t &input, const path_t &output, ShaderType shaderType);
/// Converting SpirV to OpenGL shader language for old v 330
bool compileSpvShaderToGlsl(const path_t &input, const path_t &output, ShaderType shaderType);
/// Reflect SpirV shader
bool reflect(const path_t &input, const path_t &output);

} // namespace MirenTools