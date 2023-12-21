//
// Created by Michael Andreichev on 20.12.2023.
//

#include "Panels/Common/Common.hpp"
#include "ComponentsDraw.hpp"

namespace Panda {

void drawTranslation(Transform &transform) {
    glm::vec3 position = transform.getPosition();
    drawVec3Control("Translation", position);
    transform.setPosition(position);
}

void drawRotation(Transform &transform) {
    glm::vec3 rotation = transform.getRotation();
    drawVec3Control("Rotation", rotation);
    transform.setRotation(rotation);
}

void drawComponents(Entity entity) {
    auto &transform = entity.getTransform();
    drawTranslation(transform);
    drawRotation(transform);
}

} // namespace Panda
