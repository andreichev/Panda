//
// Created by Admin on 12.02.2022.
//

#include "CameraMove.hpp"

void CameraMove::initialize() {
    transform = getEntity().getTransform();
    events = &Panda::ApplicationContext::get().getInput();
    window = &Panda::ApplicationContext::get().getWindow();
}

void CameraMove::update(float deltaTime) {
    float speed = moveSpeed * deltaTime;

    if (events->isKeyPressed(Panda::Key::W)) {
        transform->translate(Panda::Direction::Forward, speed);
    }
    if (events->isKeyPressed(Panda::Key::S)) {
        transform->translate(Panda::Direction::Backward, speed);
    }
    if (events->isKeyPressed(Panda::Key::A)) {
        transform->translate(Panda::Direction::Left, speed);
    }
    if (events->isKeyPressed(Panda::Key::D)) {
        transform->translate(Panda::Direction::Right, speed);
    }
    if (events->isKeyPressed(Panda::Key::SPACE)) {
        transform->translate(Panda::Direction::Up, speed);
    }
    if (events->isKeyPressed(Panda::Key::LEFT_SHIFT)) {
        transform->translate(Panda::Direction::Down, speed);
    }
    static float lastMouseX = events->getMousePositionX();
    static float lastMouseY = events->getMousePositionY();
    if (window->isCursorLocked()) {
        float mouseX = events->getMousePositionX();
        float mouseY = events->getMousePositionY();
        float deltaX = mouseX - lastMouseX;
        float deltaY = mouseY - lastMouseY;
        // DeltaX - смещение мыши за реальное время, поэтому умножение на deltaTime не требуется.
        // Действия в реальном мире не нужно умножать на deltaTime, умножать нужно только действия в игровом мире.
        transform->rotate(deltaY * mouseSpeed, deltaX * mouseSpeed, 0.f);
        lastMouseX = mouseX;
        lastMouseY = mouseY;
    }
}
