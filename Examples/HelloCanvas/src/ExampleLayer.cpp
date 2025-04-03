//
// Created by Admin on 19/03/2022.
//

#include "ExampleLayer.hpp"
#include "OrthographicCameraMove.hpp"

#include <Panda.hpp>
#include <Panda/GameLogic/Components/ParticleSystem.hpp>
#include <Fern/Events/KeyEvents.hpp>
#include <Fern/Events/WindowEvents.hpp>
#include <PandaUI/PandaUI.hpp>
#include <Miren/Miren.hpp>

ExampleLayer::ExampleLayer(Fern::Window *window)
    : m_window(window)
    , m_cameraMove(&m_cameraTransform)
    , m_emitter(&m_cameraTransform, &m_camera, &m_renderer2d) {}

void ExampleLayer::onAttach() {
    PandaUI::initialize();
    m_cameraTransform.translate({0.f, 0.f, 10.f});
    auto size = m_window->getSize();
    m_camera.setViewportSize({size.width, size.height});
}

void ExampleLayer::onDetach() {}

void ExampleLayer::onUpdate(double deltaTime) {
    m_renderer2d.begin();

    glm::mat4 viewMtx = glm::inverse(m_cameraTransform.getLocalTransform());
    glm::mat4 projMtx = m_camera.getProjection();
    glm::mat4 viewProjMtx = projMtx * viewMtx;
    m_renderer2d.setViewProj(viewProjMtx);

    m_cameraMove.update(deltaTime);
    m_emitter.update(deltaTime);

    m_renderer2d.end();
}

void ExampleLayer::onImGuiRender() {
    m_cameraMove.onImGuiRender();
    m_emitter.onImGuiRender();
}

void ExampleLayer::onEvent(Fern::Event *event) {
    if (event->type == Fern::EventType::QuitRequest ||
        event->type == Fern::EventType::WindowCloseRequest) {
        event->isHandled = true;
        Panda::Application::get()->close();
    } else if (event->type == Fern::EventType::WindowResize) {
        const Fern::WindowResizeEvent *ev = static_cast<const Fern::WindowResizeEvent *>(event);
        Panda::Size size = {
            static_cast<float>(ev->getWidth()), static_cast<float>(ev->getHeight())
        };
        m_camera.setViewportSize(size);
        m_window->getDrawingContext()->update();
    } else if (event->type == Fern::EventType::KeyReleased) {
        const Fern::KeyReleasedEvent *ev = static_cast<const Fern::KeyReleasedEvent *>(event);
        if (ev->key == Fern::Key::F) {
            m_window->setState(
                m_window->getState() == Fern::WindowState::WindowStateFullScreen
                    ? Fern::WindowState::WindowStateNormal
                    : Fern::WindowState::WindowStateFullScreen
            );
        } else if (ev->key == Fern::Key::ESCAPE) {
            Panda::Application::get()->close();
        }
    }
}
