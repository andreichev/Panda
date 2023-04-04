//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Panda/GameLogic/Component.hpp"
#include "Panda/GameLogic/Components/TransformDelegate.hpp"
#include "Panda/GameLogic/Primitives/MeshData.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

class Transform;

class Mesh : public Component, TransformDelegate {
public:
    ~Mesh() override;
    Mesh(const MeshData &primitiveMeshData,
        bool isDynamic,
        Miren::TextureHandle texture,
        Miren::ShaderHandle shader);
    Mesh(Miren::Vertex *vertices,
        unsigned int verticesCount,
        unsigned int *indices,
        unsigned int indicesCount,
        bool isDynamic,
        Miren::TextureHandle texture,
        Miren::ShaderHandle shader);
    void transformChanged(glm::vec4 position, glm::vec3 rotation) override;
    void initialize() override;
    void update(double deltaTime) override;
    void updateBuffer(const MeshData &data);
    void updateBuffer(Miren::Vertex *vertices,
        unsigned int verticesCount,
        unsigned int *indices,
        unsigned int indicesCount);

private:
    void updateModelMatrix();

    Miren::TextureHandle textureHandle;
    Miren::ShaderHandle shaderHandle;
    Miren::VertexLayoutHandle bufferLayoutHandle;
    Miren::IndexBufferHandle indexBufferHandle;
    Miren::VertexBufferHandle vertexBufferHandle;
    glm::mat4 model;
    uint32_t indicesCount;
    Foundation::Shared<Transform> transform;
    const bool isDynamic;
};

} // namespace Panda
