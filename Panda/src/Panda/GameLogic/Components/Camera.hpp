//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Panda/GameLogic/Component.hpp"
#include "Panda/Events/WindowSizeListener.hpp"
#include "Panda/Renderer/Shader.hpp"
#include "Transform.hpp"
#include "TransformDelegate.hpp"

namespace Panda {

class Camera : public Component, WindowSizeListener, TransformDelegate {
public:
    Camera();
    ~Camera() override;
    void initialize() override;
    void setShader(Shared<Shader> shader);
    void setFieldOfView(float degrees);
    // MARK: - Delegate
    void transformChanged(glm::vec4 position, glm::vec3 rotation) override;
    void windowSizeChanged(GSize size) override;
    void update(float deltaTime) override;

private:
    void updateProjectionMatrix();
    void updateViewMatrix();

    GSize windowSize;
    Shared<Transform> transform;
    glm::vec3 target;
    glm::mat4 view;
    glm::mat4 projection;
    Shared<Shader> shader;
    float fieldOfView;
};

} // namespace Panda
