//
// Created by Admin on 12.02.2022.
//
#include "pndpch.hpp"
#include "Mesh.hpp"

#include <utility>

#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

Mesh::Mesh(const MeshData &primitiveMeshData,
    bool isDynamic,
    Miren::TextureHandle texture,
    Miren::ShaderHandle shader)
    : isDynamic(isDynamic)
    , model(1.f)
    , indexBufferHandle()
    , vertexBufferHandle()
    , indicesCount(primitiveMeshData.indicesCount)
    , textureHandle(texture)
    , shaderHandle(shader)
    , transform(nullptr) {
    Miren::VertexBufferLayoutData layoutData;
    layoutData.pushVector();
    bufferLayoutHandle = Miren::createVertexLayout(layoutData);
    if (isDynamic) {
        vertexBufferHandle = Miren::createDynamicVertexBuffer(primitiveMeshData.vertices,
            sizeof(Miren::Vertex) * primitiveMeshData.verticesCount,
            bufferLayoutHandle);
        indexBufferHandle = Miren::createDynamicIndexBuffer(primitiveMeshData.indices,
            Miren::BufferElementType::UnsignedInt,
            primitiveMeshData.indicesCount);
    } else {
        vertexBufferHandle = Miren::createVertexBuffer(primitiveMeshData.vertices,
            sizeof(Miren::Vertex) * primitiveMeshData.verticesCount,
            bufferLayoutHandle);
        indexBufferHandle = Miren::createIndexBuffer(primitiveMeshData.indices,
            Miren::BufferElementType::UnsignedInt,
            primitiveMeshData.indicesCount);
    }
}

Mesh::Mesh(Miren::Vertex *vertices,
    uint32_t verticesCount,
    uint32_t *indices,
    uint32_t indicesCount,
    bool isDynamic,
    Miren::TextureHandle texture,
    Miren::ShaderHandle shader)
    : isDynamic(isDynamic)
    , model(1.f)
    , vertexBufferHandle()
    , indexBufferHandle()
    , indicesCount(indicesCount)
    , textureHandle(texture)
    , shaderHandle(shader)
    , transform(nullptr) {
    Miren::VertexBufferLayoutData layoutData;
    layoutData.pushVector();
    bufferLayoutHandle = Miren::createVertexLayout(layoutData);
    if (isDynamic) {
        vertexBufferHandle = Miren::createDynamicVertexBuffer(
            vertices, sizeof(Miren::Vertex) * verticesCount, bufferLayoutHandle);
        indexBufferHandle = Miren::createDynamicIndexBuffer(
            indices, Miren::BufferElementType::UnsignedInt, indicesCount);
    } else {
        vertexBufferHandle = Miren::createVertexBuffer(
            vertices, sizeof(Miren::Vertex) * verticesCount, bufferLayoutHandle);
        indexBufferHandle =
            Miren::createIndexBuffer(indices, Miren::BufferElementType::UnsignedInt, indicesCount);
    }
}

Mesh::~Mesh() {
    Miren::deleteVertexLayout(bufferLayoutHandle);
    Miren::deleteVertexBuffer(vertexBufferHandle);
    Miren::deleteIndexBuffer(indexBufferHandle);
    transform->removeDelegate(this);
}

void Mesh::initialize() {
    transform = getEntity().getTransform();
    transform->addDelegate(this);
}

void Mesh::update(double deltaTime) {
    Miren::setShader(shaderHandle);
    Miren::setUniform(shaderHandle, "model", &model[0][0], Miren::UniformDataType::Mat4);
    Miren::setTexture(textureHandle, 0);
    Miren::setVertexBuffer(vertexBufferHandle);
    Miren::setIndexBuffer(indexBufferHandle, 0, indicesCount);
    Miren::submit();
}

void Mesh::updateBuffer(const MeshData &data) {
    PND_ASSERT(isDynamic, "UPDATING AVAILABLE ONLY FOR DYNAMIC MESH");
    indicesCount = data.indicesCount;
    Miren::updateDynamicVertexBuffer(
        vertexBufferHandle, data.vertices, sizeof(Miren::Vertex) * data.verticesCount);
    Miren::updateDynamicIndexBuffer(indexBufferHandle, data.indices, data.indicesCount);
}

void Mesh::updateBuffer(
    Miren::Vertex *vertices, uint32_t verticesCount, uint32_t *indices, uint32_t _indicesCount) {
    PND_ASSERT(isDynamic, "UPDATING AVAILABLE ONLY FOR DYNAMIC MESH");
    this->indicesCount = _indicesCount;
    Miren::updateDynamicVertexBuffer(
        vertexBufferHandle, vertices, sizeof(Miren::Vertex) * verticesCount);
    Miren::updateDynamicIndexBuffer(indexBufferHandle, indices, indicesCount);
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
