//
// Created by Admin on 11.02.2022.
//

#pragma once

namespace Panda {

class OpenGLShader {
public:
    OpenGLShader(const char *vertexPath, const char *fragmentPath);
    ~OpenGLShader();
    void use();
    void setUniformMat4(const char *name, float *value);
    void setUniformInt(const char *name, int value);

private:
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
    static void checkCompileErrors(unsigned int shader, const std::string &type);
    int getUniformLocation(const std::string &name);
};

} // namespace Panda
