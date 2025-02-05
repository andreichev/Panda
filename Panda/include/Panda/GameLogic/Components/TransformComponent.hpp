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
    void setRotationEuler(glm::vec3 degrees);
    glm::vec3 getRotationEuler() const;
    void rotateEuler(glm::vec3 degrees);
    void setRotation(glm::quat quat);
    glm::quat getRotation() const;
    glm::mat4 getRotationMatrix();
    float distanceTo(TransformComponent tc);
    // ------- Position -------
    glm::vec3 getPosition() const;
    glm::vec4 getPositionHomogeneous();
    void translate(glm::vec3 offset);
    void setPosition(glm::vec3 pos);
    // ------- Scale -------
    glm::vec3 getScale() const;
    void setScale(glm::vec3 scale);
    // ------- Matrices -------
    glm::mat4 getLocalTransform() const;
    glm::mat4 getSkyTransform() const;
    // ------- Copy values --------
    void setTransform(TransformComponent &other);
    void setTransform(const glm::mat4 &transform);

private:
    glm::quat rotation;
    // rotation in degrees
    glm::vec3 rotationEuler;
    glm::vec3 position;
    glm::vec3 scale;
};

} // namespace Panda