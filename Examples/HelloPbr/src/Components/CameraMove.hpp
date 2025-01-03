//
// Created by Admin on 12.02.2022.
//

#pragma once

#include <Panda.hpp>
#include <Miren/Miren.hpp>

class CameraMove : public Panda::NativeScript {
public:
    void initialize() override;
    void update(double deltaTime) override;
    glm::vec4 getFront() {
        return m_front;
    }
    void setShader(Miren::ProgramHandle programHandle) {
        m_programHandle = programHandle;
    }

private:
    void updateVectors();

    float m_mouseSpeed = 0.2f;
    float m_moveSpeed = 70.0f;
    Panda::TransformComponent *m_transform;
    glm::mat4 m_rotationMatrix;
    glm::vec4 m_front;
    glm::vec4 m_up;
    glm::vec4 m_right;
    glm::vec3 m_target;
    Panda::Window *m_window;
    Miren::ProgramHandle m_programHandle;
};
