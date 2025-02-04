//
// Created by Admin on 12.02.2022.
//

#include "CameraMove.hpp"

#include <glm/glm.hpp>
#include <imgui.h>

void CameraMove::initialize() {
    m_transform = &getEntity().getTransform();
    m_window = Panda::Application::get()->getWindow();
    updateVectors();
}

void CameraMove::update(double deltaTime) {
    float speed = m_moveSpeed * deltaTime;
    static double lastMouseX = Panda::Input::getMousePositionX();
    static double lastMouseY = Panda::Input::getMousePositionY();
    static bool cursorStarted = false;
    if (Panda::Input::isKeyPressed(Panda::Key::W)) { m_transform->translate(m_front * speed); }
    if (Panda::Input::isKeyPressed(Panda::Key::S)) { m_transform->translate(-m_front * speed); }
    if (Panda::Input::isKeyPressed(Panda::Key::A)) { m_transform->translate(-m_right * speed); }
    if (Panda::Input::isKeyPressed(Panda::Key::D)) { m_transform->translate(m_right * speed); }
    if (Panda::Input::isKeyPressed(Panda::Key::SPACE)) { m_transform->translate(m_up * speed); }
    if (Panda::Input::isKeyPressed(Panda::Key::LEFT_SHIFT)) {
        m_transform->translate(-m_up * speed);
    }
    if (m_window->isCursorLocked()) {
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
        m_transform->rotateEuler({-deltaY * m_mouseSpeed, -deltaX * m_mouseSpeed, 0.f});
        updateVectors();
        lastMouseX = mouseX;
        lastMouseY = mouseY;
    }
    if (Panda::Input::isKeyJustPressed(Panda::Key::TAB)) {
        m_window->toggleCursorLock();
        cursorStarted = false;
    }
    ImGui::SetNextFrameWantCaptureMouse(!m_window->isCursorLocked());
}

void CameraMove::updateVectors() {
    m_rotationMatrix = m_transform->getRotationMatrix();
    m_front = m_rotationMatrix * glm::vec4(0.f, 0.f, -1.f, 1.f);
    m_right = m_rotationMatrix * glm::vec4(1.f, 0.f, 0.f, 1.f);
    m_up = m_rotationMatrix * glm::vec4(0.f, 1.f, 0.f, 1.f);
}
