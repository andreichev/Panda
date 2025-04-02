//
// Created by Admin on 12.02.2022.
//

#include "OrthographicCameraMove.hpp"

#include <Fern/Fern.hpp>

OrthographicCameraMove::OrthographicCameraMove(Panda::TransformComponent *transform)
    : m_transform(transform) {
    updateVectors();
}

void OrthographicCameraMove::update(double deltaTime) {
    using namespace Panda;
    glm::vec4 cameraPosition = m_transform->getPositionHomogeneous();
    float speed = cameraPosition.z * m_moveSpeed * deltaTime;
    bool moved = false;
    if (Input::isKeyPressed(Fern::Key::A)) {
        cameraPosition -= m_right * speed;
        moved = true;
    } else if (Input::isKeyPressed(Fern::Key::D)) {
        cameraPosition += m_right * speed;
        moved = true;
    }
    if (Input::isKeyPressed(Fern::Key::W)) {
        cameraPosition += m_up * speed;
        moved = true;
    } else if (Input::isKeyPressed(Fern::Key::S)) {
        cameraPosition -= m_up * speed;
        moved = true;
    }
    double scroll = Input::getMouseScrollY();
    if (scroll != 0) {
        float zoom = scroll;
        cameraPosition += zoom * m_front;
        cameraPosition.z = Foundation::min(cameraPosition.z, 30.f);
        cameraPosition.z = Foundation::max(cameraPosition.z, 1.f);
        moved = true;
    }
    if (moved) { m_transform->setPosition(cameraPosition); }

    glm::vec3 cameraRotation = m_transform->getRotationEuler();
    if (Input::isKeyPressed(Fern::Key::Q)) {
        cameraRotation.z += m_rotationSpeed * deltaTime;
        if (cameraRotation.z > 180.0f) { cameraRotation -= 360.0f; }
        m_transform->setRotationEuler(cameraRotation);
        updateVectors();
    } else if (Input::isKeyPressed(Fern::Key::E)) {
        cameraRotation.z -= m_rotationSpeed * deltaTime;
        if (cameraRotation.z <= -180.0f) { cameraRotation += 360.0f; }
        m_transform->setRotationEuler(cameraRotation);
        updateVectors();
    }
}

void OrthographicCameraMove::onImGuiRender() {}

void OrthographicCameraMove::updateVectors() {
    m_rotationMatrix = m_transform->getRotationMatrix();
    m_front = m_rotationMatrix * glm::vec4(0.f, 0.f, -1.f, 1.f);
    m_right = m_rotationMatrix * glm::vec4(1.f, 0.f, 0.f, 1.f);
    m_up = m_rotationMatrix * glm::vec4(0.f, 1.f, 0.f, 1.f);
}
