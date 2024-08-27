//
// Created by Admin on 12.02.2022.
//

#pragma once

#include <Panda.hpp>

namespace Panda {

class CameraController {
public:
    CameraController();
    void update(float deltaTime);
    const glm::mat4 getViewMatrix();
    const glm::mat4 getSkyViewMatrix();
    glm::vec3 getPosition();
    void setPosition(glm::vec3 position);
    glm::quat getRotation();
    void setRotation(glm::quat quat);
    void setActive(bool flag);

private:
    void updateVectors();

    bool m_isActive;
    bool m_cursorStarted;
    double m_lastMouseX;
    double m_lastMouseY;
    TransformComponent m_transform;
    glm::vec4 m_front;
    glm::vec4 m_up;
    glm::vec4 m_right;
    glm::vec3 m_target;
    float m_mouseSpeed;
    float m_moveSpeed;
};
// '/:'
// 12wedfrgtvbh
} // namespace Panda
