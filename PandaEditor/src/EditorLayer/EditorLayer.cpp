#include "EditorLayer.hpp"
#include "Panels/Dockspace.hpp"
#include "Panels/MenuBar.hpp"
#include "Panels/StatisticsPanel.hpp"
#include "Components/OrthographicCameraMove.hpp"
#include "Components/Sprite.hpp"

namespace Panda {

EditorLayer::EditorLayer()
    : m_world(nullptr)
    , m_viewport()
    , m_hierarchyPanel(nullptr) {}

void EditorLayer::onAttach() {
    m_world = NEW(Foundation::getAllocator(), World);
    m_world->initialize();
    m_viewport.init(m_world);
    m_hierarchyPanel.setWorld(m_world);
    initializeExampleWorld();
}

void EditorLayer::initializeExampleWorld() {
    Entity cameraEntity = m_world->instantiateEntity();
    cameraEntity.setName("Camera");
    auto &camera = cameraEntity.addNativeScript<OrthographicCamera>();
    auto &move = cameraEntity.addNativeScript<OrthographicCameraMove>();
    move.setCamera(&camera);
    m_viewport.setCamera(&camera);

    Entity sprite1Entity = m_world->instantiateEntity();
    sprite1Entity.setName("Orange Sprite");
    auto &sprite1 = sprite1Entity.addNativeScript<Sprite>();
    sprite1.setColor({1.0f, 0.5f, 0.2f, 1.0f});

    Entity sprite2Entity = m_world->instantiateEntity();
    sprite2Entity.setName("Cyan Sprite");
    auto &sprite2 = sprite2Entity.addNativeScript<Sprite>();
    sprite2.setColor({0.5f, 1.0f, 1.0f, 1.0f});
    sprite2Entity.getTransform().setPosition(1.f, 1.f, 0.f);
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
    m_hierarchyPanel.onImGuiRender();
    Dockspace::endImGuiDockspace();
}

void EditorLayer::onEvent(Event *event) {
    Input::onEvent(event);
}

} // namespace Panda
