#include "Panda/Renderer/Renderer3D.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Panda {

struct DrawData {
    Renderer3D::Statistics stats;
};

Miren::ViewId Renderer3D::s_viewId = 0;
Camera *Renderer3D::s_camera = nullptr;
static DrawData s_drawData;

void updateModel(Panda::Transform *transform, glm::mat4 &model) {
    glm::vec3 position = transform->getPosition();
    model = glm::translate(glm::mat4(1.f), position);
    glm::vec3 rotation = transform->getRotation();
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
}

void Renderer3D::init() {}

void Renderer3D::begin() {
    s_drawData.stats.drawCalls = 0;
}

void Renderer3D::submit(Transform *transform, StaticMesh *mesh) {
    PND_ASSERT(mesh->m_shaderHandle.isValid(), "Invalid shader for mesh");
    Miren::setShader(mesh->m_shaderHandle);
    updateModel(transform, mesh->m_model);
    Miren::setUniform(
        mesh->m_shaderHandle, "model", &mesh->m_model[0][0], Miren::UniformDataType::Mat4);
    Miren::setUniform(mesh->m_shaderHandle,
        "projViewMtx",
        (void *)&s_camera->getViewProjectionMatrix(),
        Miren::UniformDataType::Mat4);
    Miren::setTexture(mesh->m_textureHandle, 0);
    PND_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid vertex buffer for mesh");
    Miren::setVertexBuffer(mesh->m_vertexBufferHandle);
    PND_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid index buffer for mesh");
    Miren::setIndexBuffer(mesh->m_indexBufferHandle, 0, mesh->m_indicesCount);
    Miren::submit(s_viewId);
    s_drawData.stats.drawCalls += 1;
}

void Renderer3D::submit(Transform *transform, DynamicMesh *mesh) {
    PND_ASSERT(mesh->m_shaderHandle.isValid(), "Invalid shader for mesh");
    Miren::setShader(mesh->m_shaderHandle);
    updateModel(transform, mesh->m_model);
    Miren::setUniform(
        mesh->m_shaderHandle, "model", &mesh->m_model[0][0], Miren::UniformDataType::Mat4);
    Miren::setUniform(mesh->m_shaderHandle,
        "projViewMtx",
        (void *)&s_camera->getViewProjectionMatrix(),
        Miren::UniformDataType::Mat4);
    Miren::setTexture(mesh->m_textureHandle, 0);
    PND_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid vertex buffer for mesh");
    Miren::setVertexBuffer(mesh->m_vertexBufferHandle);
    PND_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid index buffer for mesh");
    Miren::setIndexBuffer(mesh->m_indexBufferHandle, 0, mesh->m_indicesCount);
    Miren::submit(s_viewId);
    s_drawData.stats.drawCalls += 1;
}

void Renderer3D::end() {}

void Renderer3D::terminate() {}

Renderer3D::Statistics Renderer3D::getStats() {
    return s_drawData.stats;
}

void Renderer3D::setViewId(Miren::ViewId id) {
    s_viewId = id;
}

void Renderer3D::setCamera(Camera *camera) {
    s_camera = camera;
}

} // namespace Panda