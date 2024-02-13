//
// Created by Admin on 12.02.2022.
//

#include "CameraMove.hpp"

#include <imgui.h>

namespace Panda {

void CameraMove::initialize() {
    m_transform = &getEntity().getTransform();
    updateVectors();
}

void CameraMove::update(double deltaTime) {
    glm::vec3 cameraRotation = m_transform->getRotationEuler();
    glm::vec3 cameraPosition = m_transform->getPosition();
    float speed = m_moveSpeed * deltaTime;
    if (Panda::Input::isKeyPressed(Panda::Key::W)) {
        m_transform->translate(m_front * speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::S)) {
        m_transform->translate(-m_front * speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::A)) {
        m_transform->translate(-m_right * speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::D)) {
        m_transform->translate(m_right * speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::SPACE)) {
        m_transform->translate(m_up * speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::LEFT_SHIFT)) {
        m_transform->translate(-m_up * speed);
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
        m_transform->rotateEuler(
            {-glm::radians(deltaY * m_mouseSpeed), -glm::radians(deltaX * m_mouseSpeed), 0.f});
        updateVectors();
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
        m_transform->translate(zoom * m_front);
    }
}

void CameraMove::updateVectors() {
    m_rotationMatrix = m_transform->getRotationMatrix();
    m_front = m_rotationMatrix * glm::vec4(0.f, 0.f, -1.f, 1.f);
    m_right = m_rotationMatrix * glm::vec4(1.f, 0.f, 0.f, 1.f);
    m_up = m_rotationMatrix * glm::vec4(0.f, 1.f, 0.f, 1.f);
}

void CameraMove::onImGuiRender() {}

} // namespace Panda