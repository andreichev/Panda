//
// Created by Michael Andreichev on 30.11.2023.
//

#pragma once

#include <Panda/GameLogic/Camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PandaUI {

class UICamera final : public Panda::Camera {
public:
    UICamera()
        : m_proj(1.f) {}

    glm::mat4 &getProjection() override {
        return m_proj;
    }

    void setViewportSize(Panda::Size size) override {
        m_viewportSize = size;
        m_proj = glm::ortho(0.f, size.width, size.height, 0.f);
    }

    Panda::Size getViewportSize() override {
        return m_viewportSize;
    }

private:
    Panda::Size m_viewportSize;
    glm::mat4 m_proj;
};

} // namespace PandaUI
