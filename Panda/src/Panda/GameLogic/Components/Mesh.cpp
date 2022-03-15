//
// Created by Admin on 12.02.2022.
//
#include "pndpch.hpp"
#include "Mesh.hpp"

#include <utility>

#include "Panda/GameLogic/Entity.hpp"
#include "Panda/Application/ApplicationContext.hpp"

namespace Panda {

Mesh::Mesh(const MeshData &primitiveMeshData, bool isDynamic,TextureHandle texture, ShaderHandle shader)
    : isDynamic(isDynamic)
    , model(1.f)
    , indexBuffer(Miren::createIndexBuffer(primitiveMeshData.indices, primitiveMeshData.indicesCount, isDynamic))
    , indicesCount(primitiveMeshData.indicesCount)
    , vertexBuffer(Miren::createVertexBuffer(primitiveMeshData.vertices, primitiveMeshData.verticesCount, isDynamic))
    , texture(texture)
    , shader(shader)
    , transform(nullptr) {}

Mesh::Mesh(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount, bool isDynamic,
    TextureHandle texture, ShaderHandle shader)
    : isDynamic(isDynamic)
    , model(1.f)
    , indexBuffer(Miren::createIndexBuffer(indices, indicesCount, isDynamic))
    , indicesCount(indicesCount)
    , vertexBuffer(Miren::createVertexBuffer(vertices, verticesCount, isDynamic))
    , texture(std::move(texture))
    , shader(std::move(shader))
    , transform(nullptr) {}

Mesh::~Mesh() {
    // TODO: Delete vertex and index buffers
    transform->removeDelegate(this);
}

void Mesh::initialize() {
    transform = getEntity().getTransform();
    transform->addDelegate(this);
}

void Mesh::update(float deltaTime) {
    Miren::setUniform(shader, "model", &model[0][0], sizeof(glm::mat4));
    Miren::setTexture(texture, 0);
    Miren::setVertexBuffer(vertexBuffer);
    Miren::setIndexBuffer(indexBuffer, indicesCount);
    Miren::submit();
}

void Mesh::updateBuffer(const MeshData &data) {
//    if (isDynamic) {
//        vertexBuffer->update(data.vertices, data.verticesCount);
//        indexBuffer->update(data.indices);
//    } else {
//        delete vertexBuffer;
//        delete indexBuffer;
//        vertexBuffer = RendererInit::createVertexBuffer(data.vertices, data.verticesCount, false);
//        indexBuffer = RendererInit::createIndexBuffer(data.indices, data.indicesCount, false);
//    }
}

void Mesh::updateBuffer(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount) {
//    if (isDynamic) {
//        vertexBuffer->update(vertices, verticesCount);
//        indexBuffer->update(indices);
//    } else {
//        delete vertexBuffer;
//        delete indexBuffer;
//        vertexBuffer = RendererInit::createVertexBuffer(vertices, verticesCount, false);
//        indexBuffer = RendererInit::createIndexBuffer(indices, indicesCount, false);
//    }
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
