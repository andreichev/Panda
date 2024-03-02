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

void drawTag(Entity entity) {
    char buffer[256];
    memset(buffer, 0, 256);
    buffer[0] = 0; // Setting the first byte to 0 makes checking if string is empty easier later.
    const std::string &tag = entity.getName();
    memcpy(buffer, tag.c_str(), tag.length());
    if (ImGui::InputText("##Tag", buffer, 256)) {
        if (buffer[0] == 0) {
            memcpy(buffer, "Unnamed Entity", 16);
        }
        entity.getComponent<TagComponent>().tag = buffer;
    }
}

void drawComponents(Entity entity) {
    auto &transform = entity.getTransform();
    drawTag(entity);
    drawTranslation(transform);
    drawRotation(transform);
}

} // namespace Panda
