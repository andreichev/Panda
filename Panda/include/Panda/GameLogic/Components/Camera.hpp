//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Panda/GameLogic/NativeScript.hpp"
#include "Panda/Events/WindowSizeListener.hpp"
#include "Transform.hpp"
#include "TransformDelegate.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

class Camera : public NativeScript, WindowSizeListener, TransformDelegate {
public:
    Camera();
    ~Camera() override;
    void initialize() override;
    void setShader(Miren::ShaderHandle shader);
    void setFieldOfView(float degrees);
    void update(double deltaTime) override;
    inline glm::vec4 getFront() const {
        return m_front;
    }
    inline glm::vec4 getUp() const {
        return m_up;
    }
    inline glm::vec4 getRight() const {
        return m_right;
    }
    // MARK: - Delegate
    void transformChanged(glm::vec4 position, glm::vec3 rotation) override;
    void windowSizeChanged(Size size) override;

private:
    void updateVectors();
    void updateProjectionMatrix();
    void updateViewMatrix();

    Size m_windowSize;
    Transform *m_transform;
    glm::mat4 m_rotationMatrix;
    glm::vec4 m_front;
    glm::vec4 m_up;
    glm::vec4 m_right;
    glm::vec3 m_target;
    glm::mat4 m_view;
    glm::mat4 m_projection;
    Miren::ShaderHandle m_shader;
    float m_fieldOfView;
};

} // namespace Panda
