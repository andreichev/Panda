//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Panda/GameLogic/Component.hpp"
#include "Panda/GameLogic/Components/TransformDelegate.hpp"
#include "Panda/GameLogic/Primitives/MeshData.hpp"
#include "Panda/Renderer/ITexture.hpp"
#include "Panda/Renderer/IShader.hpp"
#include "Panda/Renderer/IndexBuffer.hpp"
#include "Panda/Renderer/IVertexBuffer.hpp"
#include "Panda/Renderer/IRenderer.hpp"

namespace Panda {

class Transform;

class Mesh : public Component, TransformDelegate {
public:
    ~Mesh() override;
    Mesh(const MeshData &primitiveMeshData, bool isDynamic, Shared<ITexture> texture, Shared<IShader> shader);
    Mesh(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount, bool isDynamic,
        Shared<ITexture> texture, Shared<IShader> shader);
    void transformChanged(glm::vec4 position, glm::vec3 rotation) override;
    void initialize() override;
    void update(float deltaTime) override;
    void updateBuffer(const MeshData &data);
    void updateBuffer(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount);

private:
    void updateModelMatrix();

    Shared<ITexture> texture;
    Shared<IShader> shader;
    glm::mat4 model;
    IndexBuffer *indexBuffer;
    IVertexBuffer *vertexBuffer;
    Shared<Transform> transform;
    IRenderer &renderer;
    const bool isDynamic;
};

} // namespace Panda