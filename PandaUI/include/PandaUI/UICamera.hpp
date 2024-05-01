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
        : Panda::Camera({300, 200}) {}

    void updateProjectionMatrix() override {
        m_projection = glm::ortho(0.f, m_viewportSize.width, m_viewportSize.height, 0.f);
    }
};

} // namespace PandaUI
