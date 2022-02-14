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
    void update(float deltaTime) override;
    void rotate(float x, float y, float z);
    void translate(Direction direction, float units);
    void translate(float x, float y, float z);
    glm::vec4 getUp();
    glm::vec4 getFront();
    glm::vec4 getRight();
    glm::vec4 getPosition();
    glm::vec3 getRotation();
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