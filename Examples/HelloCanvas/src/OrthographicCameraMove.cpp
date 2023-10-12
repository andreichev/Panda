//
// Created by Admin on 12.02.2022.
//

#include "OrthographicCameraMove.hpp"

void OrthographicCameraMove::initialize() {
    m_transform = &getEntity().getTransform();
}

void OrthographicCameraMove::update(double deltaTime) {
    using namespace Panda;
    glm::vec4 cameraPosition = m_transform->getPosition();
    float speed = cameraPosition.z * m_moveSpeed * deltaTime;
    bool moved = false;
    if (Input::isKeyPressed(Key::A)) {
        cameraPosition -= m_camera->getRight() * speed;
        moved = true;
    } else if (Input::isKeyPressed(Key::D)) {
        cameraPosition += m_camera->getRight() * speed;
        moved = true;
    }
    if (Input::isKeyPressed(Key::W)) {
        cameraPosition += m_camera->getUp() * speed;
        moved = true;
    } else if (Input::isKeyPressed(Key::S)) {
        cameraPosition -= m_camera->getUp() * speed;
        moved = true;
    }
    double scroll = Input::getMouseScrollY();
    if (scroll != 0) {
        float zoom = scroll * 0.5;
        cameraPosition += zoom * m_camera->getFront();
        cameraPosition.z = Foundation::min(cameraPosition.z, 30.f);
        cameraPosition.z = Foundation::max(cameraPosition.z, 1.f);
        moved = true;
    }
    if (moved) {
        m_transform->setPosition(cameraPosition);
    }

    glm::vec3 cameraRotation = m_transform->getRotation();
    if (Input::isKeyPressed(Key::Q)) {
        cameraRotation.z += m_rotationSpeed * deltaTime;
        if (cameraRotation.z > 180.0f) {
            cameraRotation -= 360.0f;
        }
        m_transform->setRotation(cameraRotation);
    } else if (Input::isKeyPressed(Key::E)) {
        cameraRotation.z -= m_rotationSpeed * deltaTime;
        if (cameraRotation.z <= -180.0f) {
            cameraRotation += 360.0f;
        }
        m_transform->setRotation(cameraRotation);
    }
}

void OrthographicCameraMove::onImGuiRender() {}
