//
// Created by Admin on 12.02.2022.
//
#include "pndpch.hpp"
#include "Mesh.hpp"

#include <utility>

#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

Mesh::Mesh(const MeshData &primitiveMeshData, bool isDynamic, TextureHandle texture, ShaderHandle shader)
    : isDynamic(isDynamic)
    , model(1.f)
    , indexBufferHandle()
    , vertexBufferHandle()
    , indicesCount(primitiveMeshData.indicesCount)
    , textureHandle(texture)
    , shaderHandle(shader)
    , transform(nullptr) {
    VertexBufferLayoutData layoutData;
    layoutData.pushVector();
    bufferLayoutHandle = Miren::createVertexLayout(layoutData);
    if (isDynamic) {
        vertexBufferHandle = Miren::createDynamicVertexBuffer(
            primitiveMeshData.vertices, sizeof(Vertex) * primitiveMeshData.verticesCount, bufferLayoutHandle);
        indexBufferHandle = Miren::createDynamicIndexBuffer(primitiveMeshData.indices, primitiveMeshData.indicesCount);
    } else {
        vertexBufferHandle =
            Miren::createVertexBuffer(primitiveMeshData.vertices, sizeof(Vertex) * primitiveMeshData.verticesCount, bufferLayoutHandle);
        indexBufferHandle = Miren::createIndexBuffer(primitiveMeshData.indices, primitiveMeshData.indicesCount);
    }
}

Mesh::Mesh(Vertex *vertices, uint32_t verticesCount, uint32_t *indices, uint32_t indicesCount, bool isDynamic, TextureHandle texture,
    ShaderHandle shader)
    : isDynamic(isDynamic)
    , model(1.f)
    , vertexBufferHandle()
    , indexBufferHandle()
    , indicesCount(indicesCount)
    , textureHandle(texture)
    , shaderHandle(shader)
    , transform(nullptr) {
    VertexBufferLayoutData layoutData;
    layoutData.pushVector();
    bufferLayoutHandle = Miren::createVertexLayout(layoutData);
    if (isDynamic) {
        vertexBufferHandle = Miren::createDynamicVertexBuffer(vertices, sizeof(Vertex) * verticesCount, bufferLayoutHandle);
        indexBufferHandle = Miren::createDynamicIndexBuffer(indices, indicesCount);
    } else {
        vertexBufferHandle = Miren::createVertexBuffer(vertices, sizeof(Vertex) * verticesCount, bufferLayoutHandle);
        indexBufferHandle = Miren::createIndexBuffer(indices, indicesCount);
    }
}

Mesh::~Mesh() {
    Miren::deleteVertexBuffer(vertexBufferHandle);
    Miren::deleteIndexBuffer(indexBufferHandle);
    transform->removeDelegate(this);
}

void Mesh::initialize() {
    transform = getEntity().getTransform();
    transform->addDelegate(this);
}

void Mesh::update(float deltaTime) {
    Miren::setUniform(shaderHandle, "model", &model[0][0], sizeof(glm::mat4));
    Miren::setTexture(textureHandle, 0);
    Miren::setVertexBuffer(vertexBufferHandle);
    Miren::setIndexBuffer(indexBufferHandle, indicesCount);
    Miren::submit();
}

void Mesh::updateBuffer(const MeshData &data) {
    if (isDynamic) {
        Miren::updateDynamicVertexBuffer(vertexBufferHandle, data.vertices, sizeof(Vertex) * data.verticesCount);
        Miren::updateDynamicIndexBuffer(indexBufferHandle, data.indices, data.indicesCount);
    } else {
        Miren::deleteVertexBuffer(vertexBufferHandle);
        Miren::deleteIndexBuffer(indexBufferHandle);
        Miren::deleteVertexLayout(bufferLayoutHandle);
        VertexBufferLayoutData layoutData;
        layoutData.pushVector();
        bufferLayoutHandle = Miren::createVertexLayout(layoutData);
        vertexBufferHandle = Miren::createVertexBuffer(data.vertices, data.verticesCount, bufferLayoutHandle);
        indexBufferHandle = Miren::createIndexBuffer(data.indices, data.indicesCount);
    }
}

void Mesh::updateBuffer(Vertex *vertices, uint32_t verticesCount, uint32_t *indices, uint32_t indicesCount) {
    if (isDynamic) {
        Miren::updateDynamicVertexBuffer(vertexBufferHandle, vertices, sizeof(Vertex) * verticesCount);
        Miren::updateDynamicIndexBuffer(indexBufferHandle, indices, indicesCount);
    } else {
        Miren::deleteVertexBuffer(vertexBufferHandle);
        Miren::deleteIndexBuffer(indexBufferHandle);
        Miren::deleteVertexLayout(bufferLayoutHandle);
        VertexBufferLayoutData layoutData;
        layoutData.pushVector();
        bufferLayoutHandle = Miren::createVertexLayout(layoutData);
        vertexBufferHandle = Miren::createVertexBuffer(vertices, verticesCount, bufferLayoutHandle);
        indexBufferHandle = Miren::createIndexBuffer(indices, indicesCount);
    }
}

void Mesh::updateModelMatrix() {
    glm::vec3 position = transform->getPosition();
    model = glm::translate(glm::mat4(1.f), position);
    glm::vec3 rotation = transform->getRotation();
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
}

void Mesh::transformChanged(glm::vec4 position, glm::vec3 rotation) {
    updateModelMatrix();
}

} // namespace Panda
