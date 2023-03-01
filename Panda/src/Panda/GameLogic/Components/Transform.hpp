//
// Created by Admin on 09.02.2022.
//

#pragma once

#include "Panda/GameLogic/Component.hpp"
#include "TransformDelegate.hpp"

namespace Panda {

enum class Direction { Forward, Backward, Left, Right, Up, Down };

class Transform : public Component {
public:
    Transform();
    void initialize() override;
    void update(double deltaTime) override;
    // ------- Rotation -------
    void rotate(float x, float y, float z);
    glm::vec3 getRotation();
    void setRotation(float x, float y, float z);
    void setRotation(glm::vec3 rot);
    // ------- Position -------
    void translate(Direction direction, float value);
    void translate(float x, float y, float z);
    glm::vec4 getPosition();
    void setPosition(float x, float y, float z);
    void setPosition(glm::vec4 pos);
    // ------------------------
    glm::vec4 getUp();
    glm::vec4 getFront();
    glm::vec4 getRight();
    void addDelegate(TransformDelegate *delegate);
    void removeDelegate(TransformDelegate *delegate);

private:
    void updateVectors();
    void transformUpdated();

    glm::vec3 rotation;
    glm::vec4 position;
    std::vector<TransformDelegate *> delegates;
    glm::mat4 rotationMatrix;
    glm::vec4 front;
    glm::vec4 up;
    glm::vec4 right;
};

} // namespace Panda