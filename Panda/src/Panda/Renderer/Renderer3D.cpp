#include "Panda/Renderer/Renderer3D.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Panda {

Renderer3D::Renderer3D()
    : m_viewId(0)
    , m_viewProj(1.f)
    , m_drawData() {}

Renderer3D::Renderer3D(Panda::Renderer3D &&other)
    : m_viewId(other.m_viewId)
    , m_viewProj(other.m_viewProj)
    , m_drawData(other.m_drawData) {}

void Renderer3D::begin(Miren::ViewId id) {
    m_drawData.stats.drawCalls = 0;
    m_viewId = id;
}

void Renderer3D::submit(glm::mat4 &transform, StaticMeshAsset *mesh) {
    PND_ASSERT(mesh->m_shaderHandle.isValid(), "Invalid shader for mesh");
    Miren::setShader(mesh->m_shaderHandle);
    Miren::setUniform(
        mesh->m_shaderHandle, "model", glm::value_ptr(transform), Miren::UniformType::Mat4
    );
    Miren::setUniform(
        mesh->m_shaderHandle, "projViewMtx", glm::value_ptr(m_viewProj), Miren::UniformType::Mat4
    );
    Miren::setTexture(mesh->m_textureHandle, 0);
    PND_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid vertex buffer for mesh");
    Miren::setVertexBuffer(mesh->m_vertexBufferHandle);
    PND_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid index buffer for mesh");
    Miren::setIndexBuffer(mesh->m_indexBufferHandle, 0, mesh->m_indicesCount);
    Miren::submit(m_viewId);
    m_drawData.stats.drawCalls += 1;
}

void Renderer3D::submit(glm::mat4 &transform, DynamicMeshAsset *mesh) {
    PND_ASSERT(mesh->m_shaderHandle.isValid(), "Invalid shader for mesh");
    Miren::setShader(mesh->m_shaderHandle);
    Miren::setUniform(
        mesh->m_shaderHandle, "model", glm::value_ptr(transform), Miren::UniformType::Mat4
    );
    Miren::setUniform(
        mesh->m_shaderHandle, "projViewMtx", glm::value_ptr(m_viewProj), Miren::UniformType::Mat4
    );
    for (int i = 0; i < mesh->m_bindings.size(); i++) {
        Miren::setTexture(mesh->m_bindings[i].texture, i);
        Miren::setUniform(
            mesh->m_shaderHandle, mesh->m_bindings[i].name, &i, Miren::UniformType::Sampler
        );
    }
    PND_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid vertex buffer for mesh");
    Miren::setVertexBuffer(mesh->m_vertexBufferHandle);
    PND_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid index buffer for mesh");
    Miren::setIndexBuffer(mesh->m_indexBufferHandle, 0, mesh->m_indicesCount);
    Miren::submit(m_viewId);
    m_drawData.stats.drawCalls += 1;
}

void Renderer3D::end() {}

Renderer3D::Statistics Renderer3D::getStats() {
    return m_drawData.stats;
}

void Renderer3D::setViewProj(glm::mat4 viewProj) {
    m_viewProj = viewProj;
}

Renderer3D &Renderer3D::operator=(Renderer3D &&other) {
    m_viewId = other.m_viewId;
    m_viewProj = other.m_viewProj;
    return *this;
}

} // namespace Panda