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

void Renderer3D::submit(glm::mat4 &transform, MeshAsset *mesh) {
    auto material = mesh->getMaterialAsset();
    if (!material || !material->isValid()) {
        PND_ASSERT(false, "Invalid material for mesh");
        return;
    }
    if (!mesh->m_vertexBufferHandle.isValid()) {
        PND_ASSERT(false, "Invalid vertex buffer for mesh");
        return;
    }
    if (!mesh->m_vertexBufferHandle.isValid()) {
        PND_ASSERT(false, "Invalid index buffer for mesh");
        return;
    }
    Miren::ProgramHandle shaderHandle = material->getShaderAsset()->getMirenHandle();
    Miren::setShader(shaderHandle);
    auto bindings = material->getBindings();
    for (int i = 0; i < bindings.size(); i++) {
        Miren::setTexture(bindings[i].texture->getMirenHandle(), i);
        Miren::setUniform(shaderHandle, bindings[i].name, &i, Miren::UniformType::Sampler);
    }
    Miren::setUniform(shaderHandle, "model", glm::value_ptr(transform), Miren::UniformType::Mat4);
    Miren::setUniform(
        shaderHandle, "projViewMtx", glm::value_ptr(m_viewProj), Miren::UniformType::Mat4
    );
    Miren::setVertexBuffer(mesh->m_vertexBufferHandle);
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