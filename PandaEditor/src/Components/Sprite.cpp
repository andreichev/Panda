//
// Created by Michael Andreichev on 17.09.2023.
//

#include "Sprite.hpp"

#include <Panda/Application/Application.hpp>

namespace Panda {

void Sprite::initialize() {
    m_transform = &getEntity().getTransform();
}

void Sprite::update(double deltaTime) {
    if (!m_transform) {
        return;
    }
    Panda::Renderer2D::RectData rect;
    rect.color = m_color;
    rect.size = {1.f, 1.f};
    if (m_transform) {
        auto position = m_transform->getPosition();
        rect.origin = {position.x, position.y, position.z};
        rect.rotation = m_transform->getRotation().z;
    }
    Panda::Application::get()->getRenderer2D().drawRect(rect);
}

} // namespace Panda