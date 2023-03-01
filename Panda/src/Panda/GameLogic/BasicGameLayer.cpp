#include "BasicGameLayer.hpp"
#include "Panda/GameLogic/Input.hpp"
#include "Panda/Renderer/Renderer2D.hpp"

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
    if (m_world->getOrthographicCamera() != nullptr) {
        Renderer2D::end(m_world->getOrthographicCamera().get());
    }
    Input::nextFrame();
}

void BasicGameLayer::onImGuiRender() {
    m_world->onImGuiRender();
}

void BasicGameLayer::onEvent(Event *event) {
    Input::onEvent(event);
}

} // namespace Panda