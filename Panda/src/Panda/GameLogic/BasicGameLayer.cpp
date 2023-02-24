#include "BasicGameLayer.hpp"
#include "Panda/Events/WindowEvents.hpp"
#include "Panda/Events/KeyEvents.hpp"
#include "Panda/Events/MouseEvents.hpp"
#include "Panda/GameLogic/Input.hpp"
#include "Panda/Renderer/Renderer2D.hpp"
#include <imgui.h>

namespace Panda {

BasicGameLayer::BasicGameLayer(Level *startupLevel) {
    m_currentLevel = startupLevel;
}

void BasicGameLayer::onAttach() {
    Renderer2D::init();
    m_world = NEW(Foundation::getAllocator(), World);
    m_currentLevel->start(m_world);
}

void BasicGameLayer::onDetach() {
    DELETE(Foundation::getAllocator(), m_currentLevel);
    DELETE(Foundation::getAllocator(), m_world);
    Renderer2D::terminate();
}

void BasicGameLayer::onUpdate(double deltaTime) {
    Renderer2D::begin();
    m_world->update(deltaTime);
    Renderer2D::end();
    Input::update();
}

void BasicGameLayer::onImGuiRender() {
    m_world->onImGuiRender();
}

void BasicGameLayer::onEvent(Event *event) {
    switch (event->type) {
        case EventType::None:
            break;
        case EventType::WindowClose:
            break;
        case EventType::WindowResize: {
            const WindowResizeEvent *ev = static_cast<const WindowResizeEvent *>(event);
            Input::setWindowSize(Size(ev->getWidth(), ev->getHeight()));
            break;
        }
        case EventType::KeyPressed: {
            const KeyPressedEvent *ev = static_cast<const KeyPressedEvent *>(event);
            Input::setKeyPressed(ev->key, true);
            break;
        }
        case EventType::KeyReleased: {
            const KeyReleasedEvent *ev = static_cast<const KeyReleasedEvent *>(event);
            Input::setKeyPressed(ev->key, false);
            break;
        }
        case EventType::MouseMoved: {
            const MouseMovedEvent *ev = static_cast<const MouseMovedEvent *>(event);
            Input::postMouseChangedPosition(ev->x, ev->y);
            break;
        }
        case EventType::MouseButtonPressed: {
            const MouseKeyEvent *ev = static_cast<const MouseKeyEvent *>(event);
            Input::setMouseButtonPressed(ev->button, true);
            break;
        }
        case EventType::MouseButtonReleased: {
            const MouseKeyEvent *ev = static_cast<const MouseKeyEvent *>(event);
            Input::setMouseButtonPressed(ev->button, false);
            break;
        }
    }
}

} // namespace Panda