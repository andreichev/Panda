//
// Created by Admin on 11.02.2022.
//

#include "pndpch.hpp"
#include "Panda/GameLogic/Components/CameraComponent.hpp"

#include "Panda/Application/Application.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

const float zNear = 0.1f;
const float zFar = 1000.f;

CameraComponent::CameraComponent()
    : m_transform(nullptr)
    , m_fieldOfView(70.f)
    , m_viewportSize(Application::get()->getWindow()->getSize())
    , m_rotationMatrix(1.f)
    , m_front(0.f)
    , m_up(0.f)
    , m_right(0.f)
    , m_target(1.f)
    , m_view(1.f)
    , m_projection(1.f) {
    LOG_INFO("Camera created, viewport size: {}, {}", m_viewportSize.width, m_viewportSize.height);
}

CameraComponent::~CameraComponent() {
    if (m_transform) {
        m_transform->removeObserver(this);
    }
}

void CameraComponent::initialize() {
    m_transform = &getEntity().getTransform();
    m_transform->addObserver(this);
    updateVectors();
    updateProjectionMatrix();
    updateViewMatrix();
}

void CameraComponent::setFieldOfView(float degrees) {
    m_fieldOfView = degrees;
}

void CameraComponent::updateProjectionMatrix() {
    float aspectRatio = m_viewportSize.width / m_viewportSize.height;
    m_projection = glm::perspective<float>(glm::radians(m_fieldOfView), aspectRatio, zNear, zFar);
}

void CameraComponent::updateVectors() {
    glm::vec3 rotation = m_transform->getRotation();
    m_rotationMatrix = glm::mat4(1.f);
    m_rotationMatrix =
        glm::rotate(m_rotationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    m_rotationMatrix =
        glm::rotate(m_rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    m_rotationMatrix =
        glm::rotate(m_rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    m_rotationMatrix = glm::transpose(m_rotationMatrix);
    m_front = m_rotationMatrix * glm::vec4(0.f, 0.f, -1.f, 1.f);
    m_right = m_rotationMatrix * glm::vec4(1.f, 0.f, 0.f, 1.f);
    m_up = m_rotationMatrix * glm::vec4(0.f, 1.f, 0.f, 1.f);
}

void CameraComponent::updateViewMatrix() {
    glm::vec4 position = m_transform->getPosition();
    m_target = position + m_front;
    m_view = glm::lookAt(glm::vec3(position), m_target, glm::vec3(m_up));
}

void CameraComponent::viewportSizeChanged(Size size) {
    m_viewportSize = size;
    updateProjectionMatrix();
}

Vec3 CameraComponent::screenCoordToWorld(Vec2 screen, float distance) {
    glm::vec4 in;
    in.x = 2.f * screen.x / m_viewportSize.width - 1.f;
    in.y = 1.f - 2.f * screen.y / m_viewportSize.height;
    // То же самое ниже
    // glm::vec4 temp = m_projection * glm::vec4(0.f, 0.f, -distance, 1.f);
    // in.z = temp.z / temp.w;
    float zValue =
        -distance * -(zFar + zNear) / (zFar - zNear) - (2 * zFar * zNear) / (zFar - zNear);
    float wValue = distance;
    in.z = zValue / wValue;
    in.w = 1.f;
    glm::mat4 viewProjInverse = glm::inverse(m_viewProjection);
    glm::vec4 result = viewProjInverse * in;
    result /= result.w;
    return Vec3(result.x, result.y, result.z);
}

Vec3 CameraComponent::screenCoordToWorld(Vec3 screen) {
    glm::vec4 in;
    in.x = 2.f * screen.x / m_viewportSize.width - 1.f;
    in.y = 1.f - 2.f * screen.y / m_viewportSize.height;
    in.z = screen.z;
    in.w = 1.f;
    glm::mat4 viewProjInverse = glm::inverse(m_viewProjection);
    glm::vec4 result = viewProjInverse * in;
    result /= result.w;
    return Vec3(result.x, result.y, result.z);
}

// MARK: - Transform Delegate

void CameraComponent::transformChanged(glm::vec4 position, glm::vec3 rotation) {
    updateVectors();
    updateViewMatrix();
}

} // namespace Panda