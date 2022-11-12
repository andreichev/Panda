//
// Created by Admin on 11.02.2022.
//

#include "OpenGLShader.hpp"
#include "Miren/PlatformData.hpp"

#include <Foundation/Assert.hpp>
#include <Foundation/Logger.hpp>
#include <Foundation/PlatformDetection.hpp>

#ifdef PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PLATFORM_DESKTOP)
#    include <glad/glad.h>
#endif

#include <sstream>
#include <fstream>

namespace Miren {

OpenGLShader::OpenGLShader()
    : m_id(-1)
    , m_uniformLocationCache() {}

void OpenGLShader::create(const char *vertexPath, const char *fragmentPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    ASSERT(m_id == -1, "PROGRAM ALREADY CREATED");
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(PlatformData::get()->getResourcesPath() + vertexPath);
        fShaderFile.open(PlatformData::get()->getResourcesPath() + fragmentPath);
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
    } catch (std::ifstream::failure &e) { ASSERT(false, "SHADER::FILE {} or {} NOT SUCCESFULLY READ", vertexPath, fragmentPath); }
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
    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);

    glLinkProgram(m_id);
    checkCompileErrors(m_id, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void OpenGLShader::terminate() {
    ASSERT(m_id != -1, "PROGRAM ALREADY DELETED");
    glDeleteProgram(m_id);
    m_id = -1;
}

void OpenGLShader::checkCompileErrors(unsigned int shader, const std::string &type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            ASSERT(false, "SHADER_COMPILATION_ERROR of type: {}\n{}", type, infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            ASSERT(false, "PROGRAM_LINKING_ERROR of type: {}\n{}", type, infoLog);
        }
    }
}
int OpenGLShader::getUniformLocation(const std::string &name) {
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
        return m_uniformLocationCache[name];
    }
    int location = glGetUniformLocation(m_id, name.c_str());
    ASSERT(location != -1, "SHADER UNIFORM {} not found", name);
    m_uniformLocationCache[name] = location;
    return location;
}

void OpenGLShader::bindAttributes(VertexBufferLayoutData &layout) {
    long pointer = 0;
    for (int i = 0; i < layout.m_elementsCount; i++) {
        glEnableVertexAttribArray(i);
        glVertexAttribDivisor(i, 0);
        int type;
        switch (layout.m_elements[i].type) {
            case BufferElementType::Float:
                type = GL_FLOAT;
                break;
            case BufferElementType::UnsignedInt:
                type = GL_UNSIGNED_INT;
                break;
            case BufferElementType::UnsignedByte:
                type = GL_UNSIGNED_BYTE;
                break;
            default:
                ASSERT(false, "Buffer element type is undefined");
                break;
        }
        glVertexAttribPointer(i, layout.m_elements[i].count, type, layout.m_elements[i].normalized ? GL_TRUE : GL_FALSE, layout.m_stride,
            (const void *)pointer);
        pointer += layout.m_elements[i].count * VertexBufferElement::getSizeOfType(layout.m_elements[i].type);
    }
}

void OpenGLShader::bind() {
    glUseProgram(m_id);
}

void OpenGLShader::unbind() {
    glUseProgram(0);
}

void OpenGLShader::setUniformMat4(const char *name, float *value) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value);
}

void OpenGLShader::setUniformInt(const char *name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

} // namespace Miren
