//
// Created by Admin on 11.02.2022.
//
#include "pndpch.hpp"

#include "OpenGLShader.hpp"
#include "Panda/Application/ApplicationContext.hpp"

#ifdef PND_PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PND_PLATFORM_DESKTOP)
#    include <glad/glad.h>
#endif

namespace Panda {

OpenGLShader::OpenGLShader(const char *vertexPath, const char *fragmentPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(ApplicationContext::get().getResourcesPath() + vertexPath);
        fShaderFile.open(ApplicationContext::get().getResourcesPath() + fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure &e) { PND_ERROR("SHADER::FILE {} or {} NOT SUCCESFULLY READ", vertexPath, fragmentPath); }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // shader Program
    m_RendererID = glCreateProgram();
    glAttachShader(m_RendererID, vertex);
    glAttachShader(m_RendererID, fragment);

    glLinkProgram(m_RendererID);
    checkCompileErrors(m_RendererID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

OpenGLShader::~OpenGLShader() {
    glDeleteProgram(m_RendererID);
}

void OpenGLShader::checkCompileErrors(unsigned int shader, const std::string &type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            PND_ERROR("SHADER_COMPILATION_ERROR of type: {}\n{}", type, infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            PND_ERROR("PROGRAM_LINKING_ERROR of type: {}\n{}", type, infoLog);
        }
    }
}
int OpenGLShader::getUniformLocation(const std::string &name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1) {
        std::cout << "WARNING:SHADER:UNIFORM " << name << " not found" << std::endl;
    }
    m_UniformLocationCache[name] = location;
    return location;
}

void OpenGLShader::use() {
    glUseProgram(m_RendererID);
}

void OpenGLShader::setUniformMat4(const char *name, float *value) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value);
}

void OpenGLShader::setUniformInt(const char *name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

} // namespace Panda
