//
// Created by Admin on 12.02.2022.
//

#include "OrthographicCameraMove.hpp"

#include <Foundation/Foundation.hpp>

namespace Panda {

void OrthographicCameraMove::initialize() {
    m_transform = &getEntity().getTransform();
    m_rotation = true;
    m_zoom = 1.0;
}

void OrthographicCameraMove::update(double deltaTime) {
    using namespace Panda;
    glm::vec3 cameraRotation = m_transform->getRotation();
    glm::vec4 cameraPosition = m_transform->getPosition();
    bool moved = false;
    if (Input::isKeyPressed(Key::A)) {
        cameraPosition.x -=
            cos(glm::radians(cameraRotation.z)) * m_cameraTranslationSpeed * deltaTime;
        cameraPosition.y -=
            sin(glm::radians(cameraRotation.z)) * m_cameraTranslationSpeed * deltaTime;
        moved = true;
    } else if (Input::isKeyPressed(Key::D)) {
        cameraPosition.x +=
            cos(glm::radians(cameraRotation.z)) * m_cameraTranslationSpeed * deltaTime;
        cameraPosition.y +=
            sin(glm::radians(cameraRotation.z)) * m_cameraTranslationSpeed * deltaTime;
        moved = true;
    }

    if (Input::isKeyPressed(Key::W)) {
        cameraPosition.x +=
            -sin(glm::radians(cameraRotation.z)) * m_cameraTranslationSpeed * deltaTime;
        cameraPosition.y +=
            cos(glm::radians(cameraRotation.z)) * m_cameraTranslationSpeed * deltaTime;
        moved = true;
    } else if (Input::isKeyPressed(Key::S)) {
        cameraPosition.x -=
            -sin(glm::radians(cameraRotation.z)) * m_cameraTranslationSpeed * deltaTime;
        cameraPosition.y -=
            cos(glm::radians(cameraRotation.z)) * m_cameraTranslationSpeed * deltaTime;
        moved = true;
    }
    if (moved) {
        m_transform->setPosition(cameraPosition);
    }

    if (m_rotation) {
        if (Input::isKeyPressed(Key::Q)) {
            cameraRotation += m_cameraRotationSpeed * deltaTime;
            if (cameraRotation.z > 180.0f) {
                cameraRotation -= 360.0f;
            }
            m_transform->setRotation(cameraRotation);
        }
        if (Input::isKeyPressed(Key::E)) {
            cameraRotation -= m_cameraRotationSpeed * deltaTime;
            if (cameraRotation.z <= -180.0f) {
                cameraRotation += 360.0f;
            }
            m_transform->setRotation(cameraRotation);
        }
    }

    double scroll = Input::getMouseScrollY();
    if (scroll != 0) {
        m_zoom -= scroll * 0.15f;
        m_zoom = Foundation::min(m_zoom, 10.0);
        m_zoom = Foundation::max(m_zoom, 1.0);
        m_camera->setZoomLevel(m_zoom);
    }
    m_cameraTranslationSpeed = m_zoom;
}

void OrthographicCameraMove::onImGuiRender() {}

} // namespace Panda