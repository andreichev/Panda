#include "BasicGameLayer.hpp"
#include "Panda/GameLogic/Input.hpp"

namespace Panda {

BasicGameLayer::BasicGameLayer(Level *startupLevel) {
    m_currentLevel = startupLevel;
}

void BasicGameLayer::onAttach() {
    m_world = NEW(Foundation::getAllocator(), World);
    m_currentLevel->start(m_world);
}

void BasicGameLayer::onDetach() {
    DELETE(Foundation::getAllocator(), m_currentLevel);
    DELETE(Foundation::getAllocator(), m_world);
}

void BasicGameLayer::onUpdate(double deltaTime) {
    m_world->update(deltaTime);
    Input::nextFrame();
}

void BasicGameLayer::onImGuiRender() {
    m_world->onImGuiRender();
}

void BasicGameLayer::onEvent(Event *event) {
    Input::onEvent(event);
}

} // namespace Panda