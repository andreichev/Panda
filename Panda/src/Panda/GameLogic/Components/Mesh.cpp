//
// Created by Admin on 12.02.2022.
//

#include "pndpch.hpp"
#include "Panda/GameLogic/Components/Mesh.hpp"

#include <utility>

#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

Mesh::Mesh()
    : m_isDynamic(false)
    , m_model(1.f)
    , m_bufferLayoutHandle(MIREN_INVALID_HANDLE)
    , m_indexBufferHandle(MIREN_INVALID_HANDLE)
    , m_vertexBufferHandle(MIREN_INVALID_HANDLE)
    , m_indicesCount(0)
    , m_textureHandle(MIREN_INVALID_HANDLE)
    , m_shaderHandle(MIREN_INVALID_HANDLE)
    , m_transform(nullptr) {}

Mesh::~Mesh() {
    reset();
    m_transform->removeObserver(this);
}

void Mesh::initialize() {
    m_transform = &getEntity().getTransform();
    m_transform->addObserver(this);
}

void Mesh::reset() {
    if (m_bufferLayoutHandle.isValid()) {
        Miren::deleteVertexLayout(m_bufferLayoutHandle);
    }
    if (m_vertexBufferHandle.isValid()) {
        Miren::deleteVertexBuffer(m_vertexBufferHandle);
    }
    if (m_indexBufferHandle.isValid()) {
        Miren::deleteIndexBuffer(m_indexBufferHandle);
    }
}

void Mesh::create(const Panda::MeshData &primitiveMeshData,
    bool isDynamic,
    Miren::TextureHandle texture,
    Miren::ProgramHandle shader) {
    create(primitiveMeshData.vertices,
        primitiveMeshData.verticesCount,
        primitiveMeshData.indices,
        primitiveMeshData.indicesCount,
        isDynamic,
        texture,
        shader);
}

void Mesh::create(Miren::Vertex *vertices,
    unsigned int verticesCount,
    unsigned int *indices,
    unsigned int indicesCount,
    bool isDynamic,
    Miren::TextureHandle texture,
    Miren::ProgramHandle shader) {
    reset();
    m_shaderHandle = shader;
    m_textureHandle = texture;
    m_indicesCount = indicesCount;
    m_isDynamic = isDynamic;
    Miren::VertexBufferLayoutData layoutData;
    layoutData.pushVector();
    m_bufferLayoutHandle = Miren::createVertexLayout(layoutData);
    if (isDynamic) {
        m_vertexBufferHandle = Miren::createDynamicVertexBuffer(
            vertices, sizeof(Miren::Vertex) * verticesCount, m_bufferLayoutHandle);
        m_indexBufferHandle = Miren::createDynamicIndexBuffer(
            indices, Miren::BufferElementType::UnsignedInt, indicesCount);
    } else {
        m_vertexBufferHandle = Miren::createVertexBuffer(
            vertices, sizeof(Miren::Vertex) * verticesCount, m_bufferLayoutHandle);
        m_indexBufferHandle =
            Miren::createIndexBuffer(indices, Miren::BufferElementType::UnsignedInt, indicesCount);
    }
}

void Mesh::update(double deltaTime) {
    if (m_isActive) {
        PND_ASSERT(m_shaderHandle.isValid(), "Invalid shader for mesh");
        Miren::setShader(m_shaderHandle);
        Miren::setUniform(m_shaderHandle, "model", &m_model[0][0], Miren::UniformDataType::Mat4);
        Miren::setTexture(m_textureHandle, 0);
        PND_ASSERT(m_vertexBufferHandle.isValid(), "Invalid vertex buffer for mesh");
        Miren::setVertexBuffer(m_vertexBufferHandle);
        PND_ASSERT(m_vertexBufferHandle.isValid(), "Invalid index buffer for mesh");
        Miren::setIndexBuffer(m_indexBufferHandle, 0, m_indicesCount);
        Miren::submit(0);
    }
}

void Mesh::updateBuffer(const MeshData &data) {
    updateBuffer(data.vertices, data.verticesCount, data.indices, data.indicesCount);
}

void Mesh::updateBuffer(
    Miren::Vertex *vertices, uint32_t verticesCount, uint32_t *indices, uint32_t indicesCount) {
    PND_ASSERT(m_isDynamic, "UPDATING AVAILABLE ONLY FOR DYNAMIC MESH");
    m_indicesCount = indicesCount;
    Miren::updateDynamicVertexBuffer(
        m_vertexBufferHandle, vertices, sizeof(Miren::Vertex) * verticesCount);
    Miren::updateDynamicIndexBuffer(m_indexBufferHandle, indices, m_indicesCount);
}

void Mesh::updateModelMatrix() {
    glm::vec3 position = m_transform->getPosition();
    m_model = glm::translate(glm::mat4(1.f), position);
    glm::vec3 rotation = m_transform->getRotation();
    m_model = glm::rotate(m_model, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
    m_model = glm::rotate(m_model, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
    m_model = glm::rotate(m_model, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
}

void Mesh::transformChanged(glm::vec4 position, glm::vec3 rotation) {
    updateModelMatrix();
}

} // namespace Panda
