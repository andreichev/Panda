#include "BasicGameLayer.hpp"

namespace Panda {

BasicGameLayer::BasicGameLayer(Level *startupLevel) {
    m_currentLevel = startupLevel;
}

void BasicGameLayer::onAttach() {
    m_world = F_NEW(Foundation::getAllocator(), World);
    m_currentLevel->start(m_world);
}

void BasicGameLayer::onDetach() {
    F_DELETE(Foundation::getAllocator(), m_currentLevel);
    F_DELETE(Foundation::getAllocator(), m_world);
}

void BasicGameLayer::onUpdate(double deltaTime) {
    m_world->updateRuntime(deltaTime);
}

void BasicGameLayer::onImGuiRender() {}

void BasicGameLayer::onEvent(Event *event) {}

} // namespace Panda