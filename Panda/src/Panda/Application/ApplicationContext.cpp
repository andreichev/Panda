//
// Created by Admin on 17.02.2022.
//

#include "ApplicationContext.hpp"

#include "Application.hpp"
#include "Panda/Events/WindowEvents.hpp"
#include "Panda/Events/KeyEvents.hpp"
#include "Panda/Events/MouseEvents.hpp"
#include "Initialization/PlatformInit.hpp"
#include "Panda/Renderer/Miren.hpp"

namespace Panda {

ApplicationContext &ApplicationContext::get() {
    static ApplicationContext context;
    return context;
}

ApplicationContext::ApplicationContext() {
    isApplicationShouldClose = false;
    // Initialization not performed here
    application = new Application();
    window = createWindow();
}

ApplicationContext::~ApplicationContext() {
    delete application;
    delete window;
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
            case EventType::MouseMoved:
                const MouseMovedEvent *ev = dynamic_cast<const MouseMovedEvent *>(event);
                input.postMouseChangedPosition(ev->x, ev->y);
                break;
        }
        eventQueue.release(event);
    }
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
