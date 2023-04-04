//
// Created by Admin on 09.02.2022.
//

#pragma once

#include "Panda/GameLogic/Component.hpp"
#include "TransformDelegate.hpp"

namespace Panda {

class Transform : public Component {
public:
    Transform();
    void initialize() override;
    void update(double deltaTime) override;
    // ------- Rotation -------
    glm::vec3 getRotation();
    void rotate(float x, float y, float z);
    void setRotation(float x, float y, float z);
    void setRotation(glm::vec3 rot);
    // ------- Position -------
    glm::vec4 getPosition();
    void translate(float x, float y, float z);
    void translate(glm::vec3 offset);
    void setPosition(float x, float y, float z);
    void setPosition(glm::vec4 pos);
    // ------------------------
    void addDelegate(TransformDelegate *delegate);
    void removeDelegate(TransformDelegate *delegate);

private:
    void transformUpdated();

    glm::vec3 rotation;
    glm::vec4 position;
    std::vector<TransformDelegate *> delegates;
};

} // namespace Panda