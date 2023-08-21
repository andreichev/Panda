//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Panda/GameLogic/NativeScript.hpp"
#include "Panda/GameLogic/Components/TransformDelegate.hpp"
#include "Panda/GameLogic/Primitives/MeshData.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

class Transform;

class Mesh : public NativeScript, TransformDelegate {
public:
    ~Mesh() override;
    Mesh();
    void transformChanged(glm::vec4 position, glm::vec3 rotation) override;
    void initialize() override;
    void reset();
    void create(const MeshData &primitiveMeshData,
        bool isDynamic,
        Miren::TextureHandle texture,
        Miren::ProgramHandle shader);
    void create(Miren::Vertex *vertices,
        unsigned int verticesCount,
        unsigned int *indices,
        unsigned int indicesCount,
        bool isDynamic,
        Miren::TextureHandle texture,
        Miren::ProgramHandle shader);
    void update(double deltaTime) override;
    void updateBuffer(const MeshData &data);
    void updateBuffer(Miren::Vertex *vertices,
        unsigned int verticesCount,
        unsigned int *indices,
        unsigned int indicesCount);

private:
    void updateModelMatrix();

    Miren::TextureHandle m_textureHandle;
    Miren::ProgramHandle m_shaderHandle;
    Miren::VertexLayoutHandle m_bufferLayoutHandle;
    Miren::IndexBufferHandle m_indexBufferHandle;
    Miren::VertexBufferHandle m_vertexBufferHandle;
    glm::mat4 m_model;
    uint32_t m_indicesCount;
    Transform *m_transform;
    bool m_isDynamic;
};

} // namespace Panda
