//
// Created by Admin on 11.02.2022.
//

#include "pndpch.hpp"
#include "Camera.hpp"

#include "Panda/Application/Application.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

Camera::Camera()
    : transform(nullptr)
    , shader(0)
    , fieldOfView(70.f)
    , windowSize(Application::get()->getWindow()->getSize())
    , target(1.f)
    , view(1.f)
    , projection(1.f) {
    LOG_INFO("Camera created, viewport size: {}, {}", windowSize.width, windowSize.height);
}

Camera::~Camera() {
    transform->removeDelegate(this);
    Application::get()->removeWindowSizeListener(this);
}

void Camera::initialize() {
    transform = getEntity().getTransform();
    transform->addDelegate(this);
    Application::get()->addWindowSizeListener(this);
}

void Camera::update(double deltaTime) {
    Miren::setUniform(shader, "view", &view[0][0], Miren::UniformDataType::Mat4);
    Miren::setUniform(shader, "projection", &projection[0][0], Miren::UniformDataType::Mat4);
}

void Camera::setShader(Miren::ShaderHandle _shader) {
    this->shader = _shader;
    updateProjectionMatrix();
    updateViewMatrix();
}

void Camera::setFieldOfView(float degrees) {
    this->fieldOfView = degrees;
}

void Camera::updateProjectionMatrix() {
    projection = glm::perspective<float>(glm::radians(fieldOfView),
        (float)windowSize.width / (float)windowSize.height,
        0.1f,
        1000.0f);
    Miren::setUniform(shader, "projection", &projection[0][0], Miren::UniformDataType::Mat4);
}

void Camera::updateViewMatrix() {
    glm::vec3 position = transform->getPosition();
    glm::vec3 front = transform->getFront();
    glm::vec3 up = transform->getUp();
    target = position + front;
    view = glm::lookAt(glm::vec3(position), target, up);
    Miren::setUniform(shader, "view", &view[0][0], Miren::UniformDataType::Mat4);
}

// MARK: - Window size delegate

void Camera::windowSizeChanged(Size size) {
    windowSize = size;
    updateProjectionMatrix();
}

// MARK: - Transform Delegate

void Camera::transformChanged(glm::vec4 position, glm::vec3 rotation) {
    updateViewMatrix();
}

} // namespace Panda
