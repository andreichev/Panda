#include "MirenTools.hpp"

#include <Foundation/Foundation.hpp>
#include <sstream>

namespace MirenTools {

const std::string SPIRV_CROSS = "spirv-cross";
const std::string DXC = "dxc";

std::string getDXCShaderType(ShaderType shaderType) {
    if (shaderType == ShaderType::VERTEX) { return "vs_6_0"; }
    // FRAGMENT
    return "ps_6_0";
};

std::string getSpirvShaderType(ShaderType shaderType) {
    if (shaderType == ShaderType::VERTEX) { return "vert"; }
    // FRAGMENT
    return "frag";
};

bool compileHlslShaderToSpv(const path_t &input, const path_t &output, ShaderType shaderType) {
    std::string compileCmd = DXC + " -spirv -fspv-target-env=vulkan1.1 -T " +
                             getDXCShaderType(shaderType) + " \"" + input.string() + "\" -Fo \"" +
                             output.string() + "\"";

    LOG_INFO("EXECUTING: %s", compileCmd.c_str());
    int result = system(compileCmd.c_str());

    if (result != 0) {
        LOG_ERROR_EDITOR("Error compiling HLSL to SPIR-V (code: %d)", result);
        if (std::filesystem::exists(output)) { std::filesystem::remove(output); }
        return false;
    }

    if (std::filesystem::exists(output)) {
        LOG_INFO_EDITOR("Successfully created %s", output.c_str());
    } else {
        LOG_ERROR_EDITOR("Error: SPIR-V file was not created");
    }
    return true;
}

bool compileSpvShaderToGlsl(const path_t &input, const path_t &output, ShaderType shaderType) {
    std::string interfaceVarType;
    if (shaderType == ShaderType::VERTEX) {
        interfaceVarType = "out";
    } else {
        interfaceVarType = "in";
    }

    std::stringstream ss;
    for (int i = 0; i < 10; i++) {
        ss << "--rename-interface-variable " << interfaceVarType << " " << i << " fragVar" << i
           << " ";
    }
    std::string variableNames = ss.str();
    std::string convertCmd = SPIRV_CROSS + " --version 330 --no-420pack-extension --output \"" +
                             output.string() + "\" " + variableNames + " \"" + input.string() +
                             "\"";

    LOG_INFO("EXECUTING: %s", convertCmd.c_str());
    int result = system(convertCmd.c_str());

    if (result != 0) {
        LOG_ERROR_EDITOR("Error compiling SPIR-V to GLSL (code: %d)", result);
        return false;
    }

    if (!std::filesystem::exists(output)) {
        LOG_ERROR_EDITOR("Error: glsl file was not created");
        return false;
    }
    LOG_INFO_EDITOR("Successfully created %s", output.c_str());
    return true;
}

bool reflect(const path_t &input, const path_t &output) {
    std::string convertCmd =
        SPIRV_CROSS + " \"" + input.string() + "\" --reflect --output \"" + output.string() + "\"";
    LOG_INFO("EXECUTING: %s", convertCmd.c_str());
    int result = system(convertCmd.c_str());
    if (result != 0) {
        LOG_ERROR_EDITOR("Error reflecting SPIR-V (code: %d)", result);
        return false;
    }
    if (!std::filesystem::exists(output)) {
        LOG_ERROR_EDITOR("Error: json reflection file was not created");
        return false;
    }
    LOG_INFO_EDITOR("Successfully created %s", output.c_str());
    return true;
}

} // namespace MirenTools
