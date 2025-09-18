//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Miren/VertexBufferLayoutData.hpp"
#include "Miren/Base.hpp"

#include <string>
#include <unordered_map>

namespace Miren {

class OpenGLShader {
public:
    OpenGLShader();
    void create(ProgramCreate create);
    void terminate();
    void bind();
    void unbind();
    void setUbo(const char *name, void *value, size_t size);
    void setTexture(const char *name, uint32_t textureId);
    void bindAttributes(VertexBufferLayoutData &layout, intptr_t baseVertex);

private:
    uint32_t m_id;
    std::unordered_map<std::string, uint32_t> m_uboIndices;
    std::unordered_map<std::string, uint32_t> m_uboBindings;
    std::unordered_map<std::string, uint32_t> m_textureBindings;
    std::unordered_map<std::string, int> m_uniformLocationCache;
    std::unordered_map<std::string, uint32_t> m_uboBuffers;
    static void checkCompileErrors(uint32_t shader, const std::string &type);
    uint32_t getUboIndex(const char *name);
    uint32_t getUboBinding(const char *name);
    int getTextureBinding(const char *name);
    int getUniformLocation(const char *name);
};

} // namespace Miren
