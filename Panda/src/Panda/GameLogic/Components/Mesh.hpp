//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Panda/GameLogic/Component.hpp"
#include "Panda/GameLogic/Components/TransformDelegate.hpp"
#include "Panda/GameLogic/Primitives/MeshData.hpp"
#include "Panda/Renderer/Texture.hpp"
#include "Panda/Renderer/Shader.hpp"
#include "Panda/Renderer/IndexBuffer.hpp"
#include "Panda/Renderer/VertexBuffer.hpp"
#include "Panda/Renderer/Renderer.hpp"

namespace Panda {

class Transform;

class Mesh : public Component, TransformDelegate {
public:
    ~Mesh() override;
    Mesh(const MeshData &primitiveMeshData, bool isDynamic, Shared<Texture> texture, Shared<Shader> shader);
    Mesh(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount, bool isDynamic,
        Shared<Texture> texture, Shared<Shader> shader);
    void transformChanged(glm::vec4 position, glm::vec3 rotation) override;
    void initialize() override;
    void update(float deltaTime) override;
    void updateBuffer(const MeshData &data);
    void updateBuffer(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount);

private:
    void updateModelMatrix();

    Shared<Texture> texture;
    Shared<Shader> shader;
    glm::mat4 model;
    IndexBuffer *indexBuffer;
    VertexBuffer *vertexBuffer;
    Shared<Transform> transform;
    Renderer &renderer;
    const bool isDynamic;
};

} // namespace Panda
