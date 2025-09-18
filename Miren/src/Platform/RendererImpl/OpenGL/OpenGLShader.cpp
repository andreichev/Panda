//
// Created by Admin on 11.02.2022.
//

#include "OpenGLShader.hpp"
#include "Miren/Base.hpp"

#include "OpenGLBase.hpp"

#include <sstream>
#include <fstream>
#include <string>

namespace Miren {

OpenGLShader::OpenGLShader()
    : m_id(-1)
    , m_uniformLocationCache()
    , m_uboBindings()
    , m_uboBuffers()
    , m_uboIndices()
    , m_textureBindings() {}

void OpenGLShader::create(ProgramCreate create) {
    PND_ASSERT(m_id == -1, "PROGRAM ALREADY CREATED");
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    // LOG_INFO("\n\nVERTEX CODE: {}\n\n", vertexCode);
    // LOG_INFO("\n\nFRAGMENT CODE: {}\n\n", fragmentCode);
    const char *vertexSource = reinterpret_cast<const char *>(create.m_vertexBinary.data);
    GL_CALL(glShaderSource(vertex, 1, &vertexSource, nullptr));
    GL_CALL(glCompileShader(vertex));
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentSource = reinterpret_cast<const char *>(create.m_fragmentBinary.data);
    GL_CALL(glShaderSource(fragment, 1, &fragmentSource, nullptr));
    GL_CALL(glCompileShader(fragment));
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    m_id = glCreateProgram();
    GL_CALL(glAttachShader(m_id, vertex));
    GL_CALL(glAttachShader(m_id, fragment));
    GL_CALL(glLinkProgram(m_id));
    checkCompileErrors(m_id, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    GL_CALL(glDeleteShader(vertex));
    GL_CALL(glDeleteShader(fragment));

    create.m_vertexBinary.release();
    create.m_fragmentBinary.release();
}

void OpenGLShader::terminate() {
    PND_ASSERT(m_id != -1, "PROGRAM ALREADY DELETED");
    GL_CALL(glDeleteProgram(m_id));
    m_id = -1;
    m_textureBindings.clear();
    m_uboIndices.clear();
    m_uniformLocationCache.clear();
    m_uboBindings.clear();
    m_uboBuffers.clear();
}

void OpenGLShader::checkCompileErrors(unsigned int shader, const std::string &type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            PND_ASSERT_F(false, "SHADER_COMPILATION_ERROR of type: %s\n%s", type.c_str(), infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            PND_ASSERT_F(false, "PROGRAM_LINKING_ERROR of type: %s\n%s", type.c_str(), infoLog);
        }
    }
}

uint32_t OpenGLShader::getUboIndex(const char *name) {
    if (m_uboIndices.find(name) != m_uboIndices.end()) { return m_uboIndices[name]; }
    GLuint blockIndex = glGetUniformBlockIndex(m_id, name);
    if (blockIndex == GL_INVALID_INDEX) { LOG_ERROR("SHADER UBO %s NOT FOUND", name); }
    m_uboIndices[name] = blockIndex;
    return blockIndex;
}

uint32_t OpenGLShader::getUboBinding(const char *name) {
    if (m_uboBindings.find(name) != m_uboBindings.end()) { return m_uboBindings[name]; }
    uint32_t binding = m_uboBindings.size();
    m_uboBindings[name] = binding;
    return binding;
}

int OpenGLShader::getTextureBinding(const char *name) {
    if (m_textureBindings.find(name) != m_textureBindings.end()) { return m_textureBindings[name]; }
    int binding = m_textureBindings.size();
    m_textureBindings[name] = binding;
    return binding;
}

int OpenGLShader::getUniformLocation(const char *name) {
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
        return m_uniformLocationCache[name];
    }
    int location = glGetUniformLocation(m_id, name);
    if (location == -1) { LOG_ERROR("SHADER UNIFORM %s NOT FOUND", name); }
    m_uniformLocationCache[name] = location;
    return location;
}

void OpenGLShader::bindAttributes(VertexBufferLayoutData &layout, intptr_t baseVertex) {
    intptr_t pointer = baseVertex;
    for (int i = 0; i < layout.m_elementsCount; i++) {
        GL_CALL(glEnableVertexAttribArray(i));
        // glVertexAttribDivisor(i, 0);
        int type;
        switch (layout.m_elements[i].type) {
            case BufferElementType::Float:
                type = GL_FLOAT;
                break;
            case BufferElementType::UnsignedInt:
                type = GL_UNSIGNED_INT;
                break;
            case BufferElementType::Int:
                type = GL_INT;
                break;
            case BufferElementType::UnsignedByte:
                type = GL_UNSIGNED_BYTE;
                break;
            default:
                PND_ASSERT(false, "Buffer element type is undefined");
                break;
        }
        if (type == GL_FLOAT || layout.m_elements[i].normalized) {
            GL_CALL(glVertexAttribPointer(
                i,
                layout.m_elements[i].count,
                type,
                layout.m_elements[i].normalized ? GL_TRUE : GL_FALSE,
                layout.m_stride,
                reinterpret_cast<const void *>(pointer)
            ));
        } else {
            GL_CALL(glVertexAttribIPointer(
                i,
                layout.m_elements[i].count,
                type,
                layout.m_stride,
                reinterpret_cast<const void *>(pointer)
            ));
        }
        pointer += layout.m_elements[i].count *
                   VertexBufferElement::getSizeOfType(layout.m_elements[i].type);
    }
}

void OpenGLShader::bind() {
    GL_CALL(glUseProgram(m_id));
}

void OpenGLShader::unbind() {
    GL_CALL(glUseProgram(0));
}

void OpenGLShader::setTexture(const char *name, uint32_t textureId) {
    int binding = getTextureBinding(name);
    GL_CALL(glActiveTexture(GL_TEXTURE0 + binding));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, textureId));
    int location = getUniformLocation(name);
    if (location == -1) { return; }
    GL_CALL(glUniform1iv(location, 1, &binding));
}

void OpenGLShader::setUbo(const char *name, void *value, size_t size) {
    uint32_t index = getUboIndex(name);
    if (index == GL_INVALID_INDEX) { return; }
    uint32_t uboId;
    if (m_uboBuffers.find(name) == m_uboBuffers.end()) {
        GL_CALL(glGenBuffers(1, &uboId));
        m_uboBuffers[name] = uboId;
    }
    uboId = m_uboBuffers.at(name);
    uint32_t binding = getUboBinding(name);
    GL_CALL(glUniformBlockBinding(m_id, index, binding));
    GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, binding, uboId));
    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, uboId));
    GL_CALL(glBufferData(GL_UNIFORM_BUFFER, size, value, GL_DYNAMIC_DRAW));
}

} // namespace Miren
