//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Panda/Renderer/IShader.hpp"

namespace Panda {

class OpenGLShader : public IShader {
public:
    OpenGLShader(const char *vertexPath, const char *fragmentPath);
    ~OpenGLShader() override;
    void use() override;
    void setUniform(const char *name, glm::mat4 value) override;

private:
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
    static void checkCompileErrors(unsigned int shader, const std::string &type);
    int getUniformLocation(const std::string &name);
};

} // namespace Panda
