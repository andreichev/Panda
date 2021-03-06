//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Panda/GameLogic/Component.hpp"
#include "Panda/Events/WindowSizeListener.hpp"
#include "Panda/Renderer/Miren.hpp"
#include "Transform.hpp"
#include "TransformDelegate.hpp"

namespace Panda {

class Camera : public Component, WindowSizeListener, TransformDelegate {
public:
    Camera();
    ~Camera() override;
    void initialize() override;
    void setShader(ShaderHandle shader);
    void setFieldOfView(float degrees);
    // MARK: - Delegate
    void transformChanged(glm::vec4 position, glm::vec3 rotation) override;
    void windowSizeChanged(GSize size) override;
    void update(double deltaTime) override;

private:
    void updateProjectionMatrix();
    void updateViewMatrix();

    GSize windowSize;
    Shared<Transform> transform;
    glm::vec3 target;
    glm::mat4 view;
    glm::mat4 projection;
    ShaderHandle shader;
    float fieldOfView;
};

} // namespace Panda
