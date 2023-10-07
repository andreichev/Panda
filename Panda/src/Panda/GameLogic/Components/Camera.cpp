//
// Created by Admin on 11.02.2022.
//

#include "pndpch.hpp"
#include "Panda/GameLogic/Components/Camera.hpp"

#include "Panda/Application/Application.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

const float zNear = 0.1f;
const float zFar = 100.f;

Camera::Camera()
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

Camera::~Camera() {
    if (m_transform) {
        m_transform->removeObserver(this);
    }
}

void Camera::initialize() {
    m_transform = &getEntity().getTransform();
    m_transform->addObserver(this);
    updateVectors();
    updateProjectionMatrix();
    updateViewMatrix();
}

void Camera::setFieldOfView(float degrees) {
    m_fieldOfView = degrees;
}

void Camera::updateProjectionMatrix() {
    float aspectRatio = m_viewportSize.width / m_viewportSize.height;
    m_projection = glm::perspective<float>(glm::radians(m_fieldOfView), aspectRatio, zNear, zFar);
}

void Camera::updateVectors() {
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

void Camera::updateViewMatrix() {
    glm::vec4 position = m_transform->getPosition();
    m_target = position + m_front;
    m_view = glm::lookAt(glm::vec3(position), m_target, glm::vec3(m_up));
}

void Camera::viewportSizeChanged(Size size) {
    m_viewportSize = size;
    updateProjectionMatrix();
}

Vec3 Camera::screenCoordToWorld(Vec2 screen, float distance) {
    glm::vec4 in;
    in.x = 2.f * screen.x / m_viewportSize.width - 1.f;
    in.y = 1.f - 2.f * screen.y / m_viewportSize.height;
    /// Преобразование координаты в диапазон от -1 до +1
    ///    zNear    distance       |                    zFar
    ///      0.1      30                                1000
    ///      -1      -0.8          0                      1
    // float d = distance / (zFar - zNear);
    // in.z = -2.f * (d - 0.5f);
    in.z = 0.9f;
    in.w = 1.f;
    glm::mat4 viewProjInverse = glm::inverse(m_viewProjection);
    glm::vec4 result = viewProjInverse * in;
    result /= result.w;
    // LOG_INFO("INZ: {}, X: {}, Y: {}, Z: {}", in.z, result.x, result.y, result.z);
    return Vec3(result.x, result.y, result.z);
    // glm::vec3 result = glm::unProject(
    //     glm::vec3(screen.x, m_viewportSize.height - screen.y, 0.98f),
    //     m_view, m_projection,
    //     glm::vec4(0, 0, m_viewportSize.width, m_viewportSize.height)
    //     );
    // LOG_INFO("X: {}, Y: {}, Z: {}", result.x, result.y, result.z);
    // return Vec3(result.x, result.y, result.z);
}

// MARK: - Transform Delegate

void Camera::transformChanged(glm::vec4 position, glm::vec3 rotation) {
    updateVectors();
    updateViewMatrix();
}

} // namespace Panda
