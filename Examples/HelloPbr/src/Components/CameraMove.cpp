//
// Created by Admin on 12.02.2022.
//

#include "CameraMove.hpp"

#include <Panda/Renderer/Std140Buffer.hpp>
#include <Fern/Fern.hpp>
#include <glm/glm.hpp>
#include <imgui.h>

CameraMove::CameraMove(Panda::TransformComponent *transform)
    : m_transform(transform) {
    m_window = Panda::Application::get()->getMainWindow();
    updateVectors();
}

void CameraMove::update(double deltaTime) {
    float speed = m_moveSpeed * deltaTime;
    if (Panda::Input::isKeyPressed(Fern::Key::W)) { m_transform->translate(m_front * speed); }
    if (Panda::Input::isKeyPressed(Fern::Key::S)) { m_transform->translate(-m_front * speed); }
    if (Panda::Input::isKeyPressed(Fern::Key::A)) { m_transform->translate(-m_right * speed); }
    if (Panda::Input::isKeyPressed(Fern::Key::D)) { m_transform->translate(m_right * speed); }
    if (Panda::Input::isKeyPressed(Fern::Key::SPACE)) { m_transform->translate(m_up * speed); }
    if (Panda::Input::isKeyPressed(Fern::Key::LEFT_SHIFT)) {
        m_transform->translate(-m_up * speed);
    }
    if (Fern::isCursorLocked()) {
        double deltaX = Panda::Input::getMouseDeltaX();
        double deltaY = Panda::Input::getMouseDeltaY();
        // DeltaX - смещение мыши за реальное время, поэтому умножение на deltaTime не требуется.
        // Действия в реальном мире не нужно умножать на deltaTime, умножать нужно только действия в
        // игровом мире.
        m_transform->rotateEuler({-deltaY * m_mouseSpeed, -deltaX * m_mouseSpeed, 0.f});
        updateVectors();
    }
    if (Panda::Input::isKeyJustPressed(Fern::Key::TAB)) { Fern::toggleCursorLock(); }
    ImGui::SetNextFrameWantCaptureMouse(!Fern::isCursorLocked());
    static glm::vec3 cameraPos;
    cameraPos = m_transform->getPosition();
    Panda::Std140Buffer ubo;
    // cameraPos
    ubo.addVec3(cameraPos.x, cameraPos.y, cameraPos.z);
    Miren::addInputUniformBuffer(m_programHandle, "type_UBO_FRAG", ubo.getData(), ubo.getSize());
}

void CameraMove::updateVectors() {
    m_rotationMatrix = m_transform->getRotationMatrix();
    m_front = m_rotationMatrix * glm::vec4(0.f, 0.f, -1.f, 1.f);
    m_right = m_rotationMatrix * glm::vec4(1.f, 0.f, 0.f, 1.f);
    m_up = m_rotationMatrix * glm::vec4(0.f, 1.f, 0.f, 1.f);
}
