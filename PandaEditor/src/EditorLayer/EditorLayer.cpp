#include "EditorLayer.hpp"
#include "Panels/Dockspace.hpp"
#include "Panels/MenuBar.hpp"
#include "Panels/StatisticsPanel.hpp"

namespace Panda {

EditorLayer::EditorLayer()
    : m_world(nullptr)
    , m_viewport() {}

void EditorLayer::onAttach() {
    m_world = NEW(Foundation::getAllocator(), World);
    m_viewport.init(m_world);
}

void EditorLayer::onDetach() {
    DELETE(Foundation::getAllocator(), m_world);
}

void EditorLayer::onUpdate(double deltaTime) {
    m_world->update(deltaTime);
}

void EditorLayer::onImGuiRender() {
    Dockspace::beginImGuiDockspace();
    MenuBar::onImGuiRender();
    StatisticsPanel::onImGuiRender();
    m_viewport.onImGuiRender();
    Dockspace::endImGuiDockspace();
}

void EditorLayer::onEvent(Event *event) {
    Input::onEvent(event);
}

} // namespace Panda
