//
// Created by Admin on 11.02.2022.
//

#include "pndpch.hpp"
#include "Panda/GameLogic/Components/Camera.hpp"

#include "Panda/Application/Application.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

Camera::Camera()
    : m_transform(nullptr)
    , m_shader(0)
    , m_fieldOfView(70.f)
    , m_windowSize(Application::get()->getWindow()->getSize())
    , m_rotationMatrix(1.f)
    , m_front(0.f)
    , m_up(0.f)
    , m_right(0.f)
    , m_target(1.f)
    , m_view(1.f)
    , m_projection(1.f) {
    LOG_INFO("Camera created, viewport size: {}, {}", m_windowSize.width, m_windowSize.height);
}

Camera::~Camera() {
    m_transform->removeObserver(this);
    Application::get()->removeWindowSizeObserver(this);
}

void Camera::initialize() {
    m_transform = &getEntity().getTransform();
    m_transform->addObserver(this);
    Application::get()->addWindowSizeObserver(this);
    updateVectors();
    updateProjectionMatrix();
    updateViewMatrix();
}

void Camera::update(double deltaTime) {
    Miren::setUniform(m_shader, "view", &m_view[0][0], Miren::UniformDataType::Mat4);
    Miren::setUniform(m_shader, "projection", &m_projection[0][0], Miren::UniformDataType::Mat4);
}

void Camera::setShader(Miren::ProgramHandle _shader) {
    m_shader = _shader;
}

void Camera::setFieldOfView(float degrees) {
    m_fieldOfView = degrees;
}

void Camera::updateProjectionMatrix() {
    m_projection = glm::perspective<float>(glm::radians(m_fieldOfView),
        (float)m_windowSize.width / (float)m_windowSize.height,
        0.1f,
        1000.0f);
    Miren::setUniform(m_shader, "projection", &m_projection[0][0], Miren::UniformDataType::Mat4);
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
    Miren::setUniform(m_shader, "view", &m_view[0][0], Miren::UniformDataType::Mat4);
}

// MARK: - Window size delegate

void Camera::windowSizeChanged(Size size) {
    m_windowSize = size;
    updateProjectionMatrix();
}

// MARK: - Transform Delegate

void Camera::transformChanged(glm::vec4 position, glm::vec3 rotation) {
    updateVectors();
    updateViewMatrix();
}

} // namespace Panda
