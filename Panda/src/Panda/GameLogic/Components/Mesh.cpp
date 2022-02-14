//
// Created by Admin on 12.02.2022.
//
#include "pndpch.hpp"
#include "Mesh.hpp"

#include <utility>

#include "Panda/GameLogic/Entity.hpp"
#include "Panda/Application/Application.hpp"
#include "Panda/Platform/RendererInit.hpp"

namespace Panda {

Mesh::Mesh(const MeshData &primitiveMeshData, bool isDynamic, Shared<ITexture> texture, Shared<IShader> shader)
    : isDynamic(isDynamic)
    , model(1.f)
    , indexBuffer(RendererInit::createIndexBuffer(primitiveMeshData.indices, primitiveMeshData.indicesCount, isDynamic))
    , vertexBuffer(RendererInit::createVertexBuffer(primitiveMeshData.vertices, primitiveMeshData.verticesCount, isDynamic))
    , texture(std::move(texture))
    , shader(std::move(shader))
    , renderer(Application::get().getRenderer())
    , transform(nullptr) {}

Mesh::Mesh(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount, bool isDynamic,
    Shared<ITexture> texture, Shared<IShader> shader)
    : isDynamic(isDynamic)
    , model(1.f)
    , indexBuffer(RendererInit::createIndexBuffer(indices, indicesCount, isDynamic))
    , vertexBuffer(RendererInit::createVertexBuffer(vertices, verticesCount, isDynamic))
    , texture(std::move(texture))
    , shader(std::move(shader))
    , renderer(Application::get().getRenderer())
    , transform(nullptr) {}

Mesh::~Mesh() {
    delete indexBuffer;
    delete vertexBuffer;
    transform->removeDelegate(this);
}

void Mesh::initialize() {
    transform = getEntity().getTransform();
    transform->addDelegate(this);
}

void Mesh::update(float deltaTime) {
    shader->use();
    shader->setUniform("model", model);
    texture->bind(0);
    // Порядок биндингов для OpenGL важен
    vertexBuffer->bind();
    indexBuffer->bind();
    renderer.drawIndexed(indexBuffer->getSize());
    texture->unbind();
    indexBuffer->unbind();
    vertexBuffer->unbind();
}

void Mesh::updateBuffer(const MeshData &data) {
    if (isDynamic) {
        vertexBuffer->update(data.vertices, data.verticesCount);
        indexBuffer->update(data.indices);
    } else {
        delete vertexBuffer;
        delete indexBuffer;
        vertexBuffer = RendererInit::createVertexBuffer(data.vertices, data.verticesCount, false);
        indexBuffer = RendererInit::createIndexBuffer(data.indices, data.indicesCount, false);
    }
}

void Mesh::updateBuffer(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount) {
    if (isDynamic) {
        vertexBuffer->update(vertices, verticesCount);
        indexBuffer->update(indices);
    } else {
        delete vertexBuffer;
        delete indexBuffer;
        vertexBuffer = RendererInit::createVertexBuffer(vertices, verticesCount, false);
        indexBuffer = RendererInit::createIndexBuffer(indices, indicesCount, false);
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
