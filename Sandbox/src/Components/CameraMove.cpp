//
// Created by Admin on 12.02.2022.
//

#include "CameraMove.hpp"

#include <imgui.h>

void CameraMove::initialize() {
    transform = getEntity().getTransform();
    window = Panda::Application::get()->getWindow();
}

void CameraMove::update(double deltaTime) {
    float speed = moveSpeed * deltaTime;
    static float lastMouseX = Panda::Input::getMousePositionX();
    static float lastMouseY = Panda::Input::getMousePositionY();
    static bool cursorStarted = false;

    if (Panda::Input::isKeyPressed(Panda::Key::W)) {
        transform->translate(Panda::Direction::Forward, speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::S)) {
        transform->translate(Panda::Direction::Backward, speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::A)) {
        transform->translate(Panda::Direction::Left, speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::D)) {
        transform->translate(Panda::Direction::Right, speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::SPACE)) {
        transform->translate(Panda::Direction::Up, speed);
    }
    if (Panda::Input::isKeyPressed(Panda::Key::LEFT_SHIFT)) {
        transform->translate(Panda::Direction::Down, speed);
    }
    if (window->isCursorLocked()) {
        float mouseX = Panda::Input::getMousePositionX();
        float mouseY = Panda::Input::getMousePositionY();
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
    if (Panda::Input::isKeyJustPressed(Panda::Key::TAB)) {
        window->toggleCursorLock();
        cursorStarted = false;
    }
}

void CameraMove::onImGuiRender() {
    ImGui::Begin("Test 1", nullptr, 0);
    ImGui::Button("HELLO 1", ImVec2(200, 100));
    ImGui::Button("HELLO 2", ImVec2(200, 100));
    ImGui::End();

    ImGui::Begin("Test 2", nullptr, 0);
    ImGui::Text("GREAT! I LIKE IT!");
    ImGui::Text("Thanks thanks thanks thanks");
    ImGui::Button("SAVE", ImVec2(200, 100));
    ImGui::End();
}
