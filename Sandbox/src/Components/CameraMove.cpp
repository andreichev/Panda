//
// Created by Admin on 12.02.2022.
//

#include "CameraMove.hpp"

void CameraMove::initialize() {
    transform = getEntity().getTransform();
    events = &Panda::ApplicationContext::get()->getInput();
    window = &Panda::ApplicationContext::get()->getWindow();
}

void CameraMove::update(double deltaTime) {
    float speed = moveSpeed * deltaTime;
    static float lastMouseX = events->getMousePositionX();
    static float lastMouseY = events->getMousePositionY();
    static bool cursorStarted = false;

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
    if (window->isCursorLocked()) {
        float mouseX = events->getMousePositionX();
        float mouseY = events->getMousePositionY();
        float deltaX = mouseX - lastMouseX;
        float deltaY = mouseY - lastMouseY;
        if (cursorStarted == false) {
            cursorStarted = true;
            deltaX = 0;
            deltaY = 0;
        }
        // DeltaX - смещение мыши за реальное время, поэтому умножение на deltaTime не требуется.
        // Действия в реальном мире не нужно умножать на deltaTime, умножать нужно только действия в игровом мире.
        transform->rotate(deltaY * mouseSpeed, deltaX * mouseSpeed, 0.f);
        lastMouseX = mouseX;
        lastMouseY = mouseY;
    }
    if (events->isKeyJustPressed(Panda::Key::TAB)) {
        window->toggleCursorLock();
        cursorStarted = false;
    }
}
