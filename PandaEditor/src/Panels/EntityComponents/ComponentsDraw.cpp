//
// Created by Michael Andreichev on 20.12.2023.
//

#include "Panels/Common/Common.hpp"
#include "ComponentsDraw.hpp"

namespace Panda {

void drawTranslation(TransformComponent &transform) {
    glm::vec3 position = transform.getPosition();
    drawVec3Control("Translation", position);
    transform.setPosition(position);
}

void drawRotation(TransformComponent &transform) {
    glm::vec3 rotation = transform.getRotationEuler();
    glm::vec3 degrees = glm::degrees(rotation);
    drawVec3Control("Rotation", degrees);
    transform.setRotationEuler(glm::radians(degrees));
}

void drawComponents(Entity entity) {
    auto &transform = entity.getTransform();
    drawTranslation(transform);
    drawRotation(transform);
}

} // namespace Panda
