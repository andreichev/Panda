//
// Created by Admin on 12.02.2022.
//

#include "CameraController.hpp"

#include <imgui.h>

namespace Panda {

CameraController::CameraController()
    : m_transform()
    , m_mouseSpeed(0.2f)
    , m_moveSpeed(20.f)
    , m_front()
    , m_up()
    , m_right()
    , m_target()
    , m_cursorStarted(false)
    , m_isActive(false)
    , m_lastMouseX(0)
    , m_lastMouseY(0) {
    updateVectors();
}

void CameraController::update(float deltaTime) {
    if (!m_isActive) {
        m_cursorStarted = false;
        return;
    }
    glm::vec3 cameraRotation = m_transform.getRotationEuler();
    glm::vec3 cameraPosition = m_transform.getPosition();
    float speed = m_moveSpeed * deltaTime;
    if (Panda::Input::isKeyPressed(Panda::Key::W)) {
        m_transform.translate(m_front * speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::S)) {
        m_transform.translate(-m_front * speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::A)) {
        m_transform.translate(-m_right * speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::D)) {
        m_transform.translate(m_right * speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::SPACE)) {
        m_transform.translate(m_up * speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::LEFT_SHIFT)) {
        m_transform.translate(-m_up * speed);
    }
    if (Input::isMouseButtonPressed(MouseButton::RIGHT)) {
        double mouseX = Panda::Input::getMousePositionX();
        double mouseY = Panda::Input::getMousePositionY();
        double deltaX = mouseX - m_lastMouseX;
        double deltaY = mouseY - m_lastMouseY;
        if (!m_cursorStarted) {
            m_cursorStarted = true;
            deltaX = 0;
            deltaY = 0;
        }
        // DeltaX - смещение мыши за реальное время, поэтому умножение на deltaTime не требуется.
        // Действия в реальном мире не нужно умножать на deltaTime, умножать нужно только действия в
        // игровом мире.
        m_transform.rotateEuler({-deltaY * m_mouseSpeed, -deltaX * m_mouseSpeed, 0.f});
        updateVectors();
        m_lastMouseX = mouseX;
        m_lastMouseY = mouseY;
        // ImGui::SetNextFrameWantCaptureMouse(false);
    } else {
        // ImGui::SetNextFrameWantCaptureMouse(true);
        m_cursorStarted = false;
    }

    double scroll = Input::getMouseScrollY();
    if (scroll != 0) {
        float zoom = scroll * 0.5;
        m_transform.translate(zoom * m_front);
    }
}

void CameraController::updateVectors() {
    glm::mat4 rotationMatrix = m_transform.getRotationMatrix();
    m_front = rotationMatrix * glm::vec4(0.f, 0.f, -1.f, 1.f);
    m_right = rotationMatrix * glm::vec4(1.f, 0.f, 0.f, 1.f);
    m_up = rotationMatrix * glm::vec4(0.f, 1.f, 0.f, 1.f);
}

const glm::mat4 CameraController::getViewMatrix() {
    return glm::inverse(m_transform.getTransform());
}

const glm::mat4 CameraController::getSkyViewMatrix() {
    return glm::inverse(m_transform.getSkyTransform());
}

void CameraController::setPosition(glm::vec3 position) {
    m_transform.setPosition(position);
}

void CameraController::setActive(bool flag) {
    m_isActive = flag;
}

} // namespace Panda