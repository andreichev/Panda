//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Miren/VertexBufferLayoutData.hpp"

#include <string>
#include <unordered_map>

namespace Miren {

class OpenGLShader {
public:
    OpenGLShader();
    void create(const char *vertexPath, const char *fragmentPath);
    void terminate();
    void bind();
    void unbind();
    void setUniformMat4(const char *name, float *value);
    void setUniformInt(const char *name, int value);
    void bindAttributes(VertexBufferLayoutData &layout, intptr_t baseVertex);

private:
    uint32_t m_id;
    std::unordered_map<std::string, int> m_uniformLocationCache;
    static void checkCompileErrors(uint32_t shader, const std::string &type);
    int getUniformLocation(const std::string &name);
};

} // namespace Miren
