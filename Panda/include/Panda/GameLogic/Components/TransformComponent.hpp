//
// Created by Admin on 09.02.2022.
//

#pragma once

#include <glm/gtx/quaternion.hpp>

namespace Panda {

struct TransformComponent {
public:
    TransformComponent();
    // ------- Rotation -------
    void setRotationEuler(glm::vec3 rot);
    glm::vec3 getRotationEuler();
    void rotateEuler(glm::vec3 offset);
    void setRotation(glm::quat quat);
    glm::quat getRotation();
    glm::mat4 getRotationMatrix();
    // ------- Position -------
    glm::vec3 getPosition();
    glm::vec4 getPositionHomogeneous();
    void translate(glm::vec3 offset);
    void setPosition(glm::vec3 pos);
    glm::mat4 getTransform() const;

private:
    glm::quat rotation;
    glm::vec3 rotationEuler;
    glm::vec3 position;
    glm::vec3 scale;
};

} // namespace Panda