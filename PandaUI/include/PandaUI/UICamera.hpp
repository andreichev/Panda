//
// Created by Michael Andreichev on 30.11.2023.
//

#pragma once

#include <Panda/GameLogic/Camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PandaUI {

class UICamera : public Panda::Camera {
public:
    UICamera()
        : m_viewProj(1.f) {}

    glm::mat4 &getViewProjectionMatrix() override {
        return m_viewProj;
    }

    void updateViewportSize(Size size) {
        m_viewProj = glm::ortho(0.f, size.width, size.height, 0.f);
    }

private:
    glm::mat4 m_viewProj;
};

} // namespace PandaUI
