//
// Created by Admin on 12.02.2022.
//

#include "CameraController.hpp"

#include <Panda/Math/Math.hpp>

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
    , m_lastMouseY(0)
    , m_animation() {
    updateVectors();
}

void CameraController::update(float deltaTime) {
    if (m_animation.isActive) {
        float distanceToEnd = m_animation.endTransform.distanceTo(m_transform);
        if (distanceToEnd < 0.1) {
            m_transform.setTransform(m_animation.endTransform);
            m_animation.isActive = false;
            updateVectors();
        } else {
            static glm::vec3 velocity = glm::vec3(0.0);
            m_transform.setPosition(Math::smoothDamp(
                m_transform.getPosition(),
                m_animation.endTransform.getPosition(),
                velocity,
                0.15,
                deltaTime
            ));
            float distanceFromStart = m_animation.startTransform.distanceTo(m_transform);
            float progress = distanceFromStart / m_animation.distance;
            m_transform.setRotation(glm::mix(
                m_animation.startTransform.getRotation(),
                m_animation.endTransform.getRotation(),
                progress
            ));
        }
    }
    bool ctrl = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl);
    bool shift = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift);
    if (ctrl || shift) {
        m_cursorStarted = false;
        return;
    }
    if (!m_isActive) {
        m_cursorStarted = false;
        return;
    }
    glm::vec3 cameraRotation = m_transform.getRotationEuler();
    glm::vec3 cameraPosition = m_transform.getPosition();
    float speed = m_moveSpeed * deltaTime;
    if (ImGui::IsKeyDown(ImGuiKey_W)) { m_transform.translate(m_front * speed); }
    if (ImGui::IsKeyDown(ImGuiKey_S)) { m_transform.translate(-m_front * speed); }
    if (ImGui::IsKeyDown(ImGuiKey_A)) { m_transform.translate(-m_right * speed); }
    if (ImGui::IsKeyDown(ImGuiKey_D)) { m_transform.translate(m_right * speed); }
    if (ImGui::IsKeyDown(ImGuiKey_MouseRight)) {
        ImVec2 mousePos = ImGui::GetMousePos();
        double mouseX = mousePos.x;
        double mouseY = mousePos.y;
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
    return glm::inverse(m_transform.getLocalTransform());
}

const glm::mat4 CameraController::getSkyViewMatrix() {
    return glm::inverse(m_transform.getSkyTransform());
}

glm::vec3 CameraController::getPosition() {
    return m_transform.getPosition();
}

void CameraController::setPosition(glm::vec3 position) {
    m_transform.setPosition(position);
}

glm::quat CameraController::getRotation() {
    return m_transform.getRotation();
}

void CameraController::setRotation(glm::quat quat) {
    m_transform.setRotation(quat);
    updateVectors();
}

void CameraController::setActive(bool flag) {
    m_isActive = flag;
}

void CameraController::reset(glm::vec3 pos) {
    TransformComponent newTransform;
    newTransform.setPosition({pos.x + 1.5, pos.y + 1.5, pos.z + 1.5});
    glm::quat r;
    r.x = -0.2911117374897003;
    r.y = 0.31903332471847534;
    r.z = 0.10366018861532211;
    r.w = 0.8959499597549438;
    newTransform.setRotation(r);
    animateTo(newTransform);
}

void CameraController::animateTo(TransformComponent transform) {
    Animation animation;
    animation.startTransform = m_transform;
    animation.endTransform = transform;
    animation.isActive = true;
    animation.distance = m_transform.distanceTo(transform);
    m_animation = animation;
}

} // namespace Panda