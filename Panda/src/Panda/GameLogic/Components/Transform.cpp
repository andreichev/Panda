//
// Created by Admin on 09.02.2022.
//

#include "pndpch.hpp"
#include "Panda/GameLogic/Components/Transform.hpp"

namespace Panda {

Transform::Transform()
    : rotation()
    , position()
    , delegates() {}

void Transform::initialize() {}

void Transform::update(double deltaTime) {}

void Transform::setRotation(float x, float y, float z) {
    setRotation(glm::vec3(x, y, z));
}

void Transform::setRotation(glm::vec3 rot) {
    if (rotation == rot) {
        return;
    }
    rotation = rot;
    transformUpdated();
}

void Transform::rotate(float x, float y, float z) {
    if (x == 0 && y == 0 && z == 0) {
        return;
    }
    rotation += glm::vec3(x, y, z);
    transformUpdated();
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

void Transform::translate(glm::vec3 offset) {
    if (offset.x == 0 && offset.y == 0 && offset.z == 0) {
        return;
    }
    position += glm::vec4(offset, 1.f);
    // position += offset;
    transformUpdated();
}

void Transform::setPosition(float x, float y, float z) {
    setPosition(glm::vec4(x, y, z, 1.0f));
}

void Transform::setPosition(glm::vec4 pos) {
    if (position == pos) {
        return;
    }
    position = pos;
    transformUpdated();
}

void Transform::addDelegate(TransformDelegate *delegate) {
    delegates.push_back(delegate);
}

void Transform::removeDelegate(TransformDelegate *delegate) {
    delegates.erase(std::find(delegates.begin(), delegates.end(), delegate));
}

glm::vec4 Transform::getPosition() {
    return position;
}
glm::vec3 Transform::getRotation() {
    return rotation;
}

} // namespace Panda