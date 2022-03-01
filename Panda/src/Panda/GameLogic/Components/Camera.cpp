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
    , shader(nullptr)
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

void Camera::update(float deltaTime) {}

void Camera::setShader(Shared<Shader> shader) {
    this->shader = std::move(shader);
    updateProjectionMatrix();
    updateViewMatrix();
}

void Camera::setFieldOfView(float degrees) {
    this->fieldOfView = degrees;
}

void Camera::updateProjectionMatrix() {
    projection = glm::perspective(glm::radians(fieldOfView), windowSize.width / windowSize.height, 0.1f, 1000.0f);
    shader->use();
    shader->setUniform("projection", projection);
}

void Camera::updateViewMatrix() {
    glm::vec3 position = transform->getPosition();
    glm::vec3 front = transform->getFront();
    glm::vec3 up = transform->getUp();
    target = position + front;
    view = glm::lookAt(glm::vec3(position), target, up);
    shader->use();
    shader->setUniform("view", view);
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
