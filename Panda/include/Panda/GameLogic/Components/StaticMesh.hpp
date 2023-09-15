//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Panda/Renderer/MeshData.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

struct MeshData;

class StaticMesh {
public:
    StaticMesh()
        : m_model(1.f)
        , m_bufferLayoutHandle(MIREN_INVALID_HANDLE)
        , m_indexBufferHandle(MIREN_INVALID_HANDLE)
        , m_vertexBufferHandle(MIREN_INVALID_HANDLE)
        , m_indicesCount(0)
        , m_textureHandle(MIREN_INVALID_HANDLE)
        , m_shaderHandle(MIREN_INVALID_HANDLE) {}
    ~StaticMesh();

    void create(const MeshData &data, Miren::TextureHandle texture, Miren::ProgramHandle shader);

private:
    Miren::TextureHandle m_textureHandle;
    Miren::ProgramHandle m_shaderHandle;
    Miren::VertexLayoutHandle m_bufferLayoutHandle;
    Miren::IndexBufferHandle m_indexBufferHandle;
    Miren::VertexBufferHandle m_vertexBufferHandle;
    glm::mat4 m_model;
    uint32_t m_indicesCount;

    friend class Renderer3D;
};

} // namespace Panda
