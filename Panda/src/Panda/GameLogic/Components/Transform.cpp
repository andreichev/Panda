//
// Created by Admin on 09.02.2022.
//

#include "pndpch.hpp"
#include "Transform.hpp"

namespace Panda {

Transform::Transform()
    : rotation()
    , position()
    , rotationMatrix()
    , delegates()
    , front()
    , up()
    , right() {}

void Transform::initialize() {}

void Transform::update(float deltaTime) {}

void Transform::rotate(float x, float y, float z) {
    if (x == 0 && y == 0 && z == 0) {
        return;
    }
    rotation += glm::vec3(x, y, z);
    updateVectors();
    transformUpdated();
}

void Transform::updateVectors() {
    rotationMatrix = glm::mat4(1.f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    rotationMatrix = glm::transpose(rotationMatrix);
    front = rotationMatrix * glm::vec4(0.f, 0.f, -1.f, 1.f);
    right = rotationMatrix * glm::vec4(1.f, 0.f, 0.f, 1.f);
    up = rotationMatrix * glm::vec4(0.f, 1.f, 0.f, 1.f);
}

void Transform::transformUpdated() {
    for (auto &delegate : delegates) {
        delegate->transformChanged(position, rotation);
    }
}

void Transform::translate(float x, float y, float z) {
    if (x == 0 && y == 0 && z == 0) {
        return;
    }
    position += glm::vec4(x, y, z, 1.f);
    transformUpdated();
}

void Transform::translate(Direction direction, float units) {
    switch (direction) {
        case Direction::Forward:
            position += front * units;
            break;
        case Direction::Backward:
            position -= front * units;
            break;
        case Direction::Left:
            position -= right * units;
            break;
        case Direction::Right:
            position += right * units;
            break;
        case Direction::Up:
            position += up * units;
            break;
        case Direction::Down:
            position -= up * units;
            break;
    }
    transformUpdated();
}

void Transform::addDelegate(TransformDelegate *delegate) {
    delegates.push_back(delegate);
}

void Transform::removeDelegate(TransformDelegate *delegate) {
    delegates.erase(std::find(delegates.begin(), delegates.end(), delegate));
}

glm::vec4 Transform::getUp() {
    return up;
}

glm::vec4 Transform::getFront() {
    return front;
}

glm::vec4 Transform::getRight() {
    return right;
}

glm::vec4 Transform::getPosition() {
    return position;
}
glm::vec3 Transform::getRotation() {
    return rotation;
}

} // namespace Panda