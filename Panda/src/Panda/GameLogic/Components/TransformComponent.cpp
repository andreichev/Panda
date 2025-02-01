//
// Created by Admin on 09.02.2022.
//

#include "Panda/GameLogic/Components/TransformComponent.hpp"
#include "Panda/Math/Math.hpp"

#include <ImGuizmo.h>

namespace Panda {

TransformComponent::TransformComponent()
    : rotation(1.0f, 0.0f, 0.0f, 0.0f)
    , rotationEuler(0.0f, 0.0f, 0.0f)
    , position(0.0f, 0.0f, 0.0f)
    , scale(1.0f, 1.0f, 1.0f) {}

void TransformComponent::setRotationEuler(glm::vec3 degrees) {
    rotationEuler = glm::radians(degrees);
    rotation = glm::quat(rotationEuler);
}

void TransformComponent::rotateEuler(glm::vec3 degrees) {
    rotationEuler += glm::radians(degrees);
    rotation = glm::quat(rotationEuler);
}

glm::vec3 TransformComponent::getRotationEuler() {
    return glm::degrees(rotationEuler);
}

void TransformComponent::setRotation(glm::quat quat) {
    rotation = quat;
    rotationEuler = glm::eulerAngles(rotation);
}

glm::quat TransformComponent::getRotation() {
    return rotation;
}

glm::mat4 TransformComponent::getRotationMatrix() {
    return glm::toMat4(rotation);
}

float TransformComponent::distanceTo(TransformComponent tc) {
    return glm::distance(position, tc.position);
}

void TransformComponent::translate(glm::vec3 offset) {
    position += offset;
}

void TransformComponent::setPosition(glm::vec3 pos) {
    position = pos;
}

glm::vec3 TransformComponent::getPosition() {
    return position;
}

glm::vec4 TransformComponent::getPositionHomogeneous() {
    return {position.x, position.y, position.z, 1.0f};
}

glm::mat4 TransformComponent::getLocalTransform() const {
    return glm::translate(glm::mat4(1.0f), position) * glm::toMat4(rotation) *
           glm::scale(glm::mat4(1.0f), scale);
}

glm::mat4 TransformComponent::getSkyTransform() const {
    return glm::toMat4(rotation) * glm::scale(glm::mat4(1.0f), scale);
}

void TransformComponent::setTransform(const glm::mat4 &transform) {
    ImGuizmo::DecomposeMatrixToComponents(
        glm::value_ptr(transform),
        glm::value_ptr(position),
        glm::value_ptr(rotationEuler),
        glm::value_ptr(scale)
    );
    rotationEuler = glm::radians(rotationEuler);
    rotation = glm::quat(rotationEuler);
}

glm::vec3 TransformComponent::getScale() {
    return scale;
}

void TransformComponent::setScale(glm::vec3 _scale) {
    scale = _scale;
}

void TransformComponent::setTransform(TransformComponent &other) {
    position = other.position;
    rotation = other.rotation;
    rotationEuler = other.rotationEuler;
    scale = other.scale;
}

} // namespace Panda