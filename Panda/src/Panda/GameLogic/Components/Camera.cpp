//
// Created by Admin on 11.02.2022.
//

#include "pndpch.hpp"
#include "Camera.hpp"

#include "Panda/Application/Application.hpp"
#include "Panda/GameLogic/Entity.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Panda {

Camera::Camera()
    : transform(nullptr)
    , shader(0)
    , fieldOfView(70.f)
    , windowSize(Application::get()->getWindow()->getSize())
    , rotationMatrix(1.f)
    , front(0.f)
    , up(0.f)
    , right(0.f)
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
    updateVectors();
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

void Camera::updateVectors() {
    glm::vec3 rotation = transform->getRotation();
    rotationMatrix = glm::mat4(1.f);
    rotationMatrix =
        glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix =
        glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix =
        glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    rotationMatrix = glm::transpose(rotationMatrix);
    front = rotationMatrix * glm::vec4(0.f, 0.f, -1.f, 1.f);
    right = rotationMatrix * glm::vec4(1.f, 0.f, 0.f, 1.f);
    up = rotationMatrix * glm::vec4(0.f, 1.f, 0.f, 1.f);
}

void Camera::updateViewMatrix() {
    glm::vec4 position = transform->getPosition();
    target = position + front;
    view = glm::lookAt(glm::vec3(position), target, glm::vec3(up));
    Miren::setUniform(shader, "view", &view[0][0], Miren::UniformDataType::Mat4);
}

// MARK: - Window size delegate

void Camera::windowSizeChanged(Size size) {
    windowSize = size;
    updateProjectionMatrix();
}

// MARK: - Transform Delegate

void Camera::transformChanged(glm::vec4 position, glm::vec3 rotation) {
    updateVectors();
    updateViewMatrix();
}

} // namespace Panda
