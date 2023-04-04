//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Panda/GameLogic/Component.hpp"
#include "Panda/Events/WindowSizeListener.hpp"
#include "Transform.hpp"
#include "TransformDelegate.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

class Camera : public Component, WindowSizeListener, TransformDelegate {
public:
    Camera();
    ~Camera() override;
    void initialize() override;
    void setShader(Miren::ShaderHandle shader);
    void setFieldOfView(float degrees);
    void update(double deltaTime) override;
    inline glm::vec4 getFront() const {
        return front;
    }
    inline glm::vec4 getUp() const {
        return up;
    }
    inline glm::vec4 getRight() const {
        return right;
    }
    // MARK: - Delegate
    void transformChanged(glm::vec4 position, glm::vec3 rotation) override;
    void windowSizeChanged(Size size) override;

private:
    void updateVectors();
    void updateProjectionMatrix();
    void updateViewMatrix();

    Size windowSize;
    Foundation::Shared<Transform> transform;
    glm::mat4 rotationMatrix;
    glm::vec4 front;
    glm::vec4 up;
    glm::vec4 right;
    glm::vec3 target;
    glm::mat4 view;
    glm::mat4 projection;
    Miren::ShaderHandle shader;
    float fieldOfView;
};

} // namespace Panda
