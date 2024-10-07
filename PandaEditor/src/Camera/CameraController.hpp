//
// Created by Admin on 12.02.2022.
//

#pragma once

#include <Panda.hpp>

namespace Panda {

class CameraController {
    struct Animation {
        bool isActive = false;
        TransformComponent startTransform;
        TransformComponent endTransform;
        float distance;
        float duration;
    };

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
    void reset();
    void animateTo(TransformComponent transform);

private:
    void updateVectors();

    bool m_isActive;
    bool m_cursorStarted;
    double m_lastMouseX;
    double m_lastMouseY;
    TransformComponent m_transform;
    Animation m_animation;
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
