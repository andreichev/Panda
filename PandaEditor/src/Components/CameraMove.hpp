//
// Created by Admin on 12.02.2022.
//

#pragma once

#include <Panda.hpp>
#include <Panda/GameLogic/NativeScript.hpp>

namespace Panda {

class CameraMove : public NativeScript {
public:
    void initialize() override;
    void update(double deltaTime) override;
    void onImGuiRender() override;

private:
    void updateVectors();

    TransformComponent *m_transform;
    glm::mat4 m_rotationMatrix;
    glm::vec4 m_front;
    glm::vec4 m_up;
    glm::vec4 m_right;
    glm::vec3 m_target;
    float m_mouseSpeed = 0.2f;
    float m_moveSpeed = 20.0f;
};
// '/:'
// 12wedfrgtvbh
} // namespace Panda
