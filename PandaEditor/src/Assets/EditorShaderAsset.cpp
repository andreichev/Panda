//
// Created by Andreichev Mikhail on 26.09.2025.
//

#include "EditorShaderAsset.hpp"

#include <MirenTools.hpp>
#include <Rain/Coders/JsonDecoder.hpp>
#include <fstream>

namespace Panda {

EditorShaderAsset::EditorShaderAsset(const path_t &vertexCodePath, const path_t &fragmentCodePath) {
    path_t currentFolder = vertexCodePath.parent_path();
    path_t shaderCacheFolder = currentFolder / ".ShaderCache";
    if (!std::filesystem::exists(shaderCacheFolder)) {
        if (!std::filesystem::create_directory(shaderCacheFolder)) {
            LOG_ERROR("Error creating directory %s", shaderCacheFolder.string().c_str());
            LOG_ERROR_EDITOR("Error creating directory %s", shaderCacheFolder.string().c_str());
            return;
        }
    }
    path_t vertexFileName = vertexCodePath.filename().stem();
    path_t fragmentFileName = fragmentCodePath.filename().stem();
    path_t vertexSpvPath = shaderCacheFolder / vertexFileName.replace_extension(".spv");
    if (isInputNewer(vertexCodePath, vertexSpvPath)) {
        if (!MirenTools::compileHlslShaderToSpv(
                vertexCodePath, vertexSpvPath, MirenTools::VERTEX
            )) {
            return;
        }
    }
    path_t fragmentSpvPath = shaderCacheFolder / fragmentFileName.replace_extension(".spv");
    if (isInputNewer(fragmentCodePath, fragmentSpvPath)) {
        if (!MirenTools::compileHlslShaderToSpv(
                fragmentCodePath, fragmentSpvPath, MirenTools::FRAGMENT
            )) {
            return;
        }
    }
    path_t fragmentReflectPath = shaderCacheFolder / fragmentFileName.replace_extension(".json");
    if (isInputNewer(fragmentSpvPath, fragmentReflectPath)) {
        if (!MirenTools::reflect(fragmentSpvPath, fragmentReflectPath)) { return; }
    }
    std::ifstream file(fragmentReflectPath);
    if (!file.is_open()) {
        LOG_ERROR_EDITOR("Error reading reflection file %s", fragmentReflectPath.string().c_str());
        return;
    }
    Rain::Decoder *decoder = new Rain::JsonDecoder;
    decoder->decode(file, m_reflection);
    delete decoder;
    /// TODO: Add check if renderer is OpenGL or Vulkan. If Vulkan, return here
    path_t vertexGlslPath = shaderCacheFolder / vertexFileName.replace_extension(".glsl");
    if (isInputNewer(vertexSpvPath, vertexGlslPath)) {
        if (!MirenTools::compileSpvShaderToGlsl(
                vertexSpvPath, vertexGlslPath, MirenTools::VERTEX
            )) {
            return;
        }
    }
    path_t fragmentGlslPath = shaderCacheFolder / fragmentFileName.replace_extension(".glsl");
    if (isInputNewer(fragmentSpvPath, fragmentGlslPath)) {
        if (!MirenTools::compileSpvShaderToGlsl(
                fragmentSpvPath, fragmentGlslPath, MirenTools::FRAGMENT
            )) {
            return;
        }
    }
    create(vertexGlslPath, fragmentGlslPath);
}

ShaderSpirvReflectionData EditorShaderAsset::getReflectionData() {
    return m_reflection;
}

bool EditorShaderAsset::isInputNewer(const path_t &inputPath, const path_t &outputPath) {
    if (!std::filesystem::exists(outputPath)) { return true; }
    auto inputTime = std::filesystem::last_write_time(inputPath);
    auto outputTime = std::filesystem::last_write_time(outputPath);
    return inputTime > outputTime;
}

} // namespace Panda