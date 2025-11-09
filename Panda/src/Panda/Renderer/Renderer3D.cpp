#include "Panda/Renderer/Renderer3D.hpp"
#include "Panda/Renderer/Std140Buffer.hpp"

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

void Renderer3D::submit(glm::mat4 &model, AssetRef<MeshAsset> mesh) {
    if (!mesh) { return; }
    auto material = mesh->getMaterialAsset();
    if (!material || !material->isValid()) {
        PND_ASSERT(false, "Invalid material for mesh");
        return;
    }
    if (!mesh->m_vertexBufferHandle.isValid()) {
        PND_ASSERT(false, "Invalid vertex buffer for mesh");
        return;
    }
    if (!mesh->m_indexBufferHandle.isValid()) {
        PND_ASSERT(false, "Invalid index buffer for mesh");
        return;
    }
    Miren::ProgramHandle shaderHandle = material->getShaderAsset()->getMirenHandle();
    Miren::setShader(shaderHandle);
    material->bindFields();
    Std140Buffer ubo;
    // projViewMtx
    ubo.addMat4(glm::value_ptr(m_viewProj));
    // modelMtx
    ubo.addMat4(glm::value_ptr(model));
    Miren::addInputUniformBuffer(shaderHandle, "type_UBO_VERT", ubo.getData(), ubo.getSize());
    Miren::setVertexBuffer(mesh->m_vertexBufferHandle);
    Miren::setIndexBuffer(mesh->m_indexBufferHandle, 0, mesh->m_indicesCount);
    Miren::submit(m_viewId);
    m_drawData.stats.drawCalls += 1;
}

void Renderer3D::submitSky(glm::mat4 &skyViewProj, AssetRef<MeshAsset> skyMesh) {
    if (!skyMesh) { return; }
    auto material = skyMesh->getMaterialAsset();
    if (!material || !material->isValid()) {
        PND_ASSERT(false, "Invalid material for mesh");
        return;
    }
    if (!skyMesh->m_vertexBufferHandle.isValid()) {
        PND_ASSERT(false, "Invalid vertex buffer for mesh");
        return;
    }
    if (!skyMesh->m_indexBufferHandle.isValid()) {
        PND_ASSERT(false, "Invalid index buffer for mesh");
        return;
    }
    Miren::ProgramHandle shaderHandle = material->getShaderAsset()->getMirenHandle();
    Miren::setShader(shaderHandle);
    material->bindFields();
    Std140Buffer ubo;
    // projViewMtx
    ubo.addMat4(glm::value_ptr(skyViewProj));
    // modelMtx
    static glm::mat4 model(1.f);
    ubo.addMat4(glm::value_ptr(model));
    Miren::addInputUniformBuffer(shaderHandle, "type_UBO_VERT", ubo.getData(), ubo.getSize());
    Miren::setVertexBuffer(skyMesh->m_vertexBufferHandle);
    Miren::setIndexBuffer(skyMesh->m_indexBufferHandle, 0, skyMesh->m_indicesCount);
    Miren::setState(MIREN_STATE_CULL_FACE);
    Miren::submit(m_viewId);
    m_drawData.stats.drawCalls += 1;
}

void Renderer3D::end() {
    flush();
}

void Renderer3D::flush() {}

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