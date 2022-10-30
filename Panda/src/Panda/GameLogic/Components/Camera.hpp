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
    // MARK: - Delegate
    void transformChanged(glm::vec4 position, glm::vec3 rotation) override;
    void windowSizeChanged(Size size) override;
    void update(double deltaTime) override;

private:
    void updateProjectionMatrix();
    void updateViewMatrix();

    Size windowSize;
    Foundation::Shared<Transform> transform;
    glm::vec3 target;
    glm::mat4 view;
    glm::mat4 projection;
    Miren::ShaderHandle shader;
    float fieldOfView;
};

} // namespace Panda
