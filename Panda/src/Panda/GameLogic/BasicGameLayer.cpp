#include "BasicGameLayer.hpp"
#include "Panda/Events/WindowEvents.hpp"
#include "Panda/Events/KeyEvents.hpp"
#include "Panda/Events/MouseEvents.hpp"
#include "Panda/GameLogic/Input.hpp"
#include <imgui.h>

namespace Panda {

BasicGameLayer::BasicGameLayer(Level *startupLevel) {
    m_currentLevel = startupLevel;
}

void BasicGameLayer::onAttach() {
    m_world = new World();
    m_currentLevel->start(m_world);
}

void BasicGameLayer::onDetach() {
    delete m_currentLevel;
    delete m_world;
}

void BasicGameLayer::onUpdate(double deltaTime) {
    m_world->update(deltaTime);
    Input::update();
}

void BasicGameLayer::onImGuiRender() {
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

void BasicGameLayer::onEvent(Event *event) {
    switch (event->type) {
        case EventType::None:
            break;
        case EventType::WindowClose:
            break;
        case EventType::WindowResize: {
            const WindowResizeEvent *ev = dynamic_cast<const WindowResizeEvent *>(event);
            Input::setWindowSize(Size(ev->getWidth(), ev->getHeight()));
            break;
        }
        case EventType::KeyPressed: {
            const KeyPressedEvent *ev = dynamic_cast<const KeyPressedEvent *>(event);
            Input::setKeyPressed(ev->key, true);
            break;
        }
        case EventType::KeyReleased: {
            const KeyReleasedEvent *ev = dynamic_cast<const KeyReleasedEvent *>(event);
            Input::setKeyPressed(ev->key, false);
            break;
        }
        case EventType::MouseMoved: {
            const MouseMovedEvent *ev = dynamic_cast<const MouseMovedEvent *>(event);
            Input::postMouseChangedPosition(ev->x, ev->y);
            break;
        }
        case EventType::MouseButtonPressed: {
            const MouseKeyEvent *ev = dynamic_cast<const MouseKeyEvent *>(event);
            Input::setMouseButtonPressed(ev->button, true);
            break;
        }
        case EventType::MouseButtonReleased: {
            const MouseKeyEvent *ev = dynamic_cast<const MouseKeyEvent *>(event);
            Input::setMouseButtonPressed(ev->button, false);
            break;
        }
    }
}

} // namespace Panda