//
// Created by Admin on 09.02.2022.
//

#pragma once

#include "TransformObserver.hpp"

namespace Panda {

struct Transform {
public:
    Transform();
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
    void addObserver(TransformObserver *delegate);
    void removeObserver(TransformObserver *delegate);

private:
    void transformUpdated();

    glm::vec3 rotation;
    glm::vec4 position;
    std::vector<TransformObserver *> delegates;
};

} // namespace Panda