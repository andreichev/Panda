//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Panda/GameLogic/Component.hpp"
#include "Panda/GameLogic/Components/TransformDelegate.hpp"
#include "Panda/GameLogic/Primitives/MeshData.hpp"
#include "Panda/Renderer/Miren.hpp"

namespace Panda {

class Transform;

class Mesh : public Component, TransformDelegate {
public:
    ~Mesh() override;
    Mesh(const MeshData &primitiveMeshData, bool isDynamic, TextureHandle texture, ShaderHandle shader);
    Mesh(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount, bool isDynamic,
        TextureHandle texture, ShaderHandle shader);
    void transformChanged(glm::vec4 position, glm::vec3 rotation) override;
    void initialize() override;
    void update(float deltaTime) override;
    void updateBuffer(const MeshData &data);
    void updateBuffer(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount);

private:
    void updateModelMatrix();

    TextureHandle textureHandle;
    ShaderHandle shaderHandle;
    VertexLayoutHandle bufferLayoutHandle;
    IndexBufferHandle indexBufferHandle;
    VertexBufferHandle vertexBufferHandle;
    glm::mat4 model;
    uint32_t indicesCount;
    Shared<Transform> transform;
    const bool isDynamic;
};

} // namespace Panda
