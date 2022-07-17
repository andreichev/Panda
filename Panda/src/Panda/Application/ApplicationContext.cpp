//
// Created by Admin on 17.02.2022.
//

#include "ApplicationContext.hpp"

#include "Application.hpp"
#include "Panda/Events/WindowEvents.hpp"
#include "Panda/Events/KeyEvents.hpp"
#include "Panda/Events/MouseEvents.hpp"
#include "Initialization/PlatformInit.hpp"

namespace Panda {

ApplicationContext* ApplicationContext::s_instance;

ApplicationContext *ApplicationContext::get() {
    if (ApplicationContext::s_instance == nullptr) {
        PND_ERROR("APP CONTEXT NOT EXISTS");
    }
    return s_instance;
}

ApplicationContext::ApplicationContext() {
    s_instance = this;
}

ApplicationContext::~ApplicationContext() {
    s_instance = nullptr;
}

void ApplicationContext::runApplication(ApplicationStartupSettings &settings) {
    Logger::init();
    isApplicationShouldClose = false;
    window = createWindow(settings);
    application = new Application(settings);
    application->loop();
    delete window;
    delete application;
}

void ApplicationContext::setResourcesPath(std::string path) {
    resourcesPath = path + "/";
}

std::string &ApplicationContext::getResourcesPath() {
    return resourcesPath;
}

void ApplicationContext::processEvents() {
    const Event *event;
    while ((event = eventQueue.poll()) != nullptr) {
        switch (event->type) {
            case EventType::None:
                break;
            case EventType::WindowResize: {
                const WindowResizeEvent *ev = dynamic_cast<const WindowResizeEvent *>(event);
                input.windowSizeChanged(GSize(ev->getWidth(), ev->getHeight()));
                break;
            }
            case EventType::KeyPressed: {
                const KeyPressedEvent *ev = dynamic_cast<const KeyPressedEvent *>(event);
                input.setKeyPressed(ev->key, true);
                break;
            }
            case EventType::KeyReleased: {
                const KeyReleasedEvent *ev = dynamic_cast<const KeyReleasedEvent *>(event);
                input.setKeyPressed(ev->key, false);
                break;
            }
            case EventType::MouseMoved: {
                const MouseMovedEvent *ev = dynamic_cast<const MouseMovedEvent *>(event);
                input.postMouseChangedPosition(ev->x, ev->y);
                break;
            }
            case EventType::MouseButtonPressed: {
                const MouseKeyEvent *ev = dynamic_cast<const MouseKeyEvent *>(event);
                input.setMouseButtonPressed(ev->button, true);
                break;
            }
            case EventType::MouseButtonReleased: {
                const MouseKeyEvent *ev = dynamic_cast<const MouseKeyEvent *>(event);
                input.setMouseButtonPressed(ev->button, false);
                break;
            }
        }
        eventQueue.release(event);
    }
    input.eventProcessingFinished();
}

void ApplicationContext::postMouseButtonEvent(MouseButton button, bool pressed) {
    eventQueue.post(new MouseKeyEvent(button, pressed));
}

void ApplicationContext::postSizeEvent(unsigned int width, unsigned int height) {
    eventQueue.post(new WindowResizeEvent(width, height));
}

void ApplicationContext::postKeyEvent(Key key, bool down) {
    Event *ev;
    if (down) {
        ev = new KeyPressedEvent(key);
    } else {
        ev = new KeyReleasedEvent(key);
    }
    eventQueue.post(ev);
}

void ApplicationContext::postMouseEvent(int x, int y) {
    eventQueue.post(new MouseMovedEvent(x, y));
}

Input &ApplicationContext::getInput() {
    return input;
}

Window &ApplicationContext::getWindow() {
    return *window;
}

Application &ApplicationContext::getApplication() {
    return *application;
}

} // namespace Panda
