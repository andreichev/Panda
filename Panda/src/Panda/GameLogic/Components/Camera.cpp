//
// Created by Admin on 11.02.2022.
//

#include "pndpch.hpp"
#include "Camera.hpp"

#include "Panda/Application/ApplicationContext.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

Camera::Camera()
    : transform(nullptr)
    , shader(0)
    , fieldOfView(70.f)
    , windowSize(ApplicationContext::get().getInput().getWindowSize())
    , target(1.f)
    , view(1.f)
    , projection(1.f) {}

Camera::~Camera() {
    transform->removeDelegate(this);
    ApplicationContext::get().getInput().removeWindowSizeListener(this);
}

void Camera::initialize() {
    transform = getEntity().getTransform();
    transform->addDelegate(this);
    ApplicationContext::get().getInput().addWindowSizeListener(this);
}

void Camera::update(double deltaTime) {
    Miren::setUniform(shader, "view", &view[0][0], sizeof(glm::mat4));
    Miren::setUniform(shader, "projection", &projection[0][0], sizeof(glm::mat4));
}

void Camera::setShader(ShaderHandle _shader) {
    this->shader = _shader;
    updateProjectionMatrix();
    updateViewMatrix();
}

void Camera::setFieldOfView(float degrees) {
    this->fieldOfView = degrees;
}

void Camera::updateProjectionMatrix() {
    projection = glm::perspective(glm::radians(fieldOfView), windowSize.width / windowSize.height, 0.1f, 1000.0f);
    Miren::setUniform(shader, "projection", &projection[0][0], sizeof(glm::mat4));
}

void Camera::updateViewMatrix() {
    glm::vec3 position = transform->getPosition();
    glm::vec3 front = transform->getFront();
    glm::vec3 up = transform->getUp();
    target = position + front;
    view = glm::lookAt(glm::vec3(position), target, up);
    Miren::setUniform(shader, "view", &view[0][0], sizeof(glm::mat4));
}

// MARK: - Window size delegate

void Camera::windowSizeChanged(GSize size) {
    windowSize = size;
    updateProjectionMatrix();
}

// MARK: - Transform Delegate

void Camera::transformChanged(glm::vec4 position, glm::vec3 rotation) {
    updateViewMatrix();
}

} // namespace Panda
