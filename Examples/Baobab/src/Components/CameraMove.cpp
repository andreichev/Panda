//
// Created by Admin on 12.02.2022.
//

#include "CameraMove.hpp"

#include <glm/glm.hpp>
#include <imgui.h>

void CameraMove::initialize() {
    m_transform = getEntity().getTransform();
    m_window = Panda::Application::get()->getWindow();
}

void CameraMove::update(double deltaTime) {
    float speed = m_moveSpeed * deltaTime;
    static double lastMouseX = Panda::Input::getMousePositionX();
    static double lastMouseY = Panda::Input::getMousePositionY();
    static bool cursorStarted = false;
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
    if (m_window->isCursorLocked()) {
        double mouseX = Panda::Input::getMousePositionX();
        double mouseY = Panda::Input::getMousePositionY();
        double deltaX = mouseX - lastMouseX;
        double deltaY = mouseY - lastMouseY;
        if (cursorStarted == false) {
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
    }
    if (Panda::Input::isKeyJustPressed(Panda::Key::TAB)) {
        m_window->toggleCursorLock();
        cursorStarted = false;
    }
}

void CameraMove::onImGuiRender() {
    // ImGui::ShowDemoWindow();
}