//
// Created by Admin on 11.02.2022.
//

#include "pndpch.hpp"
#include "Panda/GameLogic/Components/WorldCamera.hpp"

#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

static const float zNearDefault = 0.1f;
static const float zFarDefault = 1000.f;

WorldCamera::WorldCamera()
    : m_viewportSize(200, 300)
    , m_projection(1.f)
    , m_fieldOfView(70.f)
    , m_projectionType(ProjectionType::PERSPECTIVE)
    , m_near(zNearDefault)
    , m_far(zFarDefault)
    , m_orthoSize(1.f) {
    LOG_INFO("Camera created, viewport size: {}, {}", m_viewportSize.width, m_viewportSize.height);
}

void WorldCamera::setFieldOfView(float degrees) {
    m_fieldOfView = degrees;
    if (m_projectionType == ProjectionType::PERSPECTIVE) {
        updateProjectionMatrix();
    }
}

void WorldCamera::setViewportSize(Panda::Size size) {
    m_viewportSize = size;
    updateProjectionMatrix();
}

void WorldCamera::updateProjectionMatrix() {
    float aspectRatio = m_viewportSize.width / m_viewportSize.height;
    switch (m_projectionType) {
        case ProjectionType::PERSPECTIVE: {
            m_projection =
                glm::perspective<float>(glm::radians(m_fieldOfView), aspectRatio, m_near, m_far);
            break;
        }
        case ProjectionType::ORTHOGRAPHIC:
            float orthoLeft = -m_orthoSize * aspectRatio * 0.5f;
            float orthoRight = m_orthoSize * aspectRatio * 0.5f;
            float orthoBottom = -m_orthoSize * 0.5f;
            float orthoTop = m_orthoSize * 0.5f;

            m_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_near, m_far);
            break;
    }
}

void WorldCamera::setProjectionType(WorldCamera::ProjectionType type) {
    m_projectionType = type;
    updateProjectionMatrix();
}

Vec3 WorldCamera::screenCoordToWorld(glm::mat4 view, Vec2 screen, float distance) {
    glm::vec4 in;
    in.x = 2.f * screen.x / m_viewportSize.width - 1.f;
    in.y = 1.f - 2.f * screen.y / m_viewportSize.height;
    // То же самое ниже
    // glm::vec4 temp = m_projection * glm::vec4(0.f, 0.f, -distance, 1.f);
    // in.z = temp.z / temp.w;
    float zFar = m_far;
    float zNear = m_near;
    float zValue =
        -distance * -(zFar + zNear) / (zFar - zNear) - (2 * zFar * zNear) / (zFar - zNear);
    float wValue = distance;
    in.z = zValue / wValue;
    in.w = 1.f;
    glm::mat4 viewProjection = m_projection * view;
    glm::mat4 viewProjInverse = glm::inverse(viewProjection);
    glm::vec4 result = viewProjInverse * in;
    result /= result.w;
    return Vec3(result.x, result.y, result.z);
}

Vec3 WorldCamera::screenCoordToWorld(glm::mat4 view, Vec3 screen) {
    glm::vec4 in;
    in.x = 2.f * screen.x / m_viewportSize.width - 1.f;
    in.y = 1.f - 2.f * screen.y / m_viewportSize.height;
    in.z = screen.z;
    in.w = 1.f;
    glm::mat4 viewProjection = m_projection * view;
    glm::mat4 viewProjInverse = glm::inverse(viewProjection);
    glm::vec4 result = viewProjInverse * in;
    result /= result.w;
    return Vec3(result.x, result.y, result.z);
}

void WorldCamera::setOrthoSize(float orthoSize) {
    m_orthoSize = orthoSize;
    if (m_projectionType == ProjectionType::ORTHOGRAPHIC) {
        updateProjectionMatrix();
    }
}

void WorldCamera::setFar(float far) {
    m_far = far;
    updateProjectionMatrix();
}

void WorldCamera::setNear(float near) {
    m_near = near;
    updateProjectionMatrix();
}

float WorldCamera::getFieldOfView() {
    return m_fieldOfView;
}

glm::mat4 &WorldCamera::getProjection() {
    return m_projection;
}

float WorldCamera::getNear() {
    return m_near;
}

float WorldCamera::getFar() {
    return m_far;
}

float WorldCamera::getOrthoSize() {
    return m_orthoSize;
}

} // namespace Panda
