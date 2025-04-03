//
// Created by Admin on 19/03/2022.
//

#include "ExampleLayer.hpp"
#include "RootView.hpp"

#include <Fern/Events/KeyEvents.hpp>
#include <Fern/Events/WindowEvents.hpp>
#include <Panda.hpp>
#include <PandaUI/PandaUI.hpp>

ExampleLayer::ExampleLayer() {}

void ExampleLayer::onAttach() {
    PandaUI::initialize();
    Foundation::Shared<RootView> view = PandaUI::makeView<RootView>();
    PandaUI::Context::shared().setRootView(view);
}

void ExampleLayer::onDetach() {}

void ExampleLayer::onUpdate(double deltaTime) {}

void ExampleLayer::onImGuiRender() {}

void ExampleLayer::onEvent(Fern::Event *event) {
    Fern::Window *window = Panda::Application::get()->getMainWindow();
    if (event->type == Fern::EventType::QuitRequest ||
        event->type == Fern::EventType::WindowCloseRequest) {
        event->isHandled = true;
        Panda::Application::get()->close();
    } else if (event->type == Fern::EventType::WindowResize) {
        window->getDrawingContext()->update();
        const Fern::WindowResizeEvent *ev = static_cast<const Fern::WindowResizeEvent *>(event);
        Panda::Size size = {
            static_cast<float>(ev->getWidth()), static_cast<float>(ev->getHeight())
        };
        Miren::Rect viewport = Miren::Rect(
            0, 0, size.width * window->getDpi().width, size.height * window->getDpi().height
        );
        Miren::setViewport(0, viewport);
    } else if (event->type == Fern::EventType::KeyReleased) {
        const Fern::KeyReleasedEvent *ev = static_cast<const Fern::KeyReleasedEvent *>(event);
        if (ev->key == Fern::Key::F) {
            window->setState(
                window->getState() == Fern::WindowState::WindowStateFullScreen
                    ? Fern::WindowState::WindowStateNormal
                    : Fern::WindowState::WindowStateFullScreen
            );
        } else if (ev->key == Fern::Key::ESCAPE) {
            Panda::Application::get()->close();
        }
    }
}
