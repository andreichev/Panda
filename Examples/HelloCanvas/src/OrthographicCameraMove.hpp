//
// Created by Admin on 12.02.2022.
//

#pragma once

#include <Panda.hpp>

class OrthographicCameraMove : public Panda::NativeScript {
public:
    void initialize() override;
    void update(double deltaTime) override;
    void onImGuiRender() override;

private:
    void updateVectors();

    Panda::TransformComponent *m_transform;
    glm::mat4 m_rotationMatrix;
    glm::vec4 m_front;
    glm::vec4 m_up;
    glm::vec4 m_right;
    glm::vec3 m_target;
    float m_moveSpeed = 1.4f;
    float m_rotationSpeed = 180.0f;
};
