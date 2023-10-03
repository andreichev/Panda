//
// Created by Admin on 12.02.2022.
//

#include "CameraMove.hpp"

#include <Foundation/Foundation.hpp>
#include <imgui.h>

namespace Panda {

void CameraMove::initialize() {
    m_transform = &getEntity().getTransform();
}

void CameraMove::update(double deltaTime) {
    glm::vec3 cameraRotation = m_transform->getRotation();
    glm::vec4 cameraPosition = m_transform->getPosition();
    float speed = m_moveSpeed * deltaTime;
    if (Panda::Input::isKeyPressed(Panda::Key::W)) {
        m_transform->translate(m_camera->getFront() * speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::S)) {
        m_transform->translate(-m_camera->getFront() * speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::A)) {
        m_transform->translate(-m_camera->getRight() * speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::D)) {
        m_transform->translate(m_camera->getRight() * speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::SPACE)) {
        m_transform->translate(m_camera->getUp() * speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::LEFT_SHIFT)) {
        m_transform->translate(-m_camera->getUp() * speed);
    }
    static double lastMouseX = Panda::Input::getMousePositionX();
    static double lastMouseY = Panda::Input::getMousePositionY();
    static bool cursorStarted = false;
    if (Input::isMouseButtonPressed(MouseButton::RIGHT)) {
        double mouseX = Panda::Input::getMousePositionX();
        double mouseY = Panda::Input::getMousePositionY();
        double deltaX = mouseX - lastMouseX;
        double deltaY = mouseY - lastMouseY;
        if (!cursorStarted) {
            cursorStarted = true;
            deltaX = 0;
            deltaY = 0;
        }
        // DeltaX - смещение мыши за реальное время, поэтому умножение на deltaTime не требуется.
        // Действия в реальном мире не нужно умножать на deltaTime, умножать нужно только действия в
        // игровом мире.
        m_transform->rotate(deltaY * m_mouseSpeed, deltaX * m_mouseSpeed, 0.f);
        lastMouseX = mouseX;
        lastMouseY = mouseY;
        ImGui::SetNextFrameWantCaptureMouse(false);
    } else {
        ImGui::SetNextFrameWantCaptureMouse(true);
        cursorStarted = false;
    }

    double scroll = Input::getMouseScrollY();
    if (scroll != 0) {
        float zoom = scroll * 0.5;
        m_transform->translate(zoom * m_camera->getFront());
    }
}

void CameraMove::onImGuiRender() {}

} // namespace Panda