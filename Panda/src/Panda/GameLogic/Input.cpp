//
// Created by Admin on 05.02.2022.
//

#include "Input.hpp"
#include "Panda/Events/WindowEvents.hpp"
#include "Panda/Events/KeyEvents.hpp"
#include "Panda/Events/MouseEvents.hpp"

namespace Panda {

uint32_t Input::frame;
bool Input::keys[1024];
uint32_t Input::framesKeys[1024];
bool Input::mouseButtons[8];
uint32_t Input::framesMouseButtons[4];
Size Input::windowSize;
int Input::mousePositionX;
int Input::mousePositionY;

void Input::onEvent(Event *event) {
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

void Input::setKeyPressed(Key key, bool state) {
    keys[(int)key] = state;
    framesKeys[(int)key] = frame + 1;
}

void Input::setMouseButtonPressed(MouseButton mouseButton, bool state) {
    mouseButtons[(int)mouseButton] = state;
    framesMouseButtons[(int)mouseButton] = frame + 1;
}

bool Input::isKeyPressed(Key key) {
    return keys[(int)key];
}

bool Input::isKeyJustPressed(Key key) {
    return keys[(int)key] && framesKeys[(int)key] == frame;
}

bool Input::isMouseButtonPressed(MouseButton mouseButton) {
    return mouseButtons[(int)mouseButton];
}

bool Input::isMouseButtonJustPressed(MouseButton mouseButton) {
    return mouseButtons[(int)mouseButton] && framesMouseButtons[(int)mouseButton] == frame;
}

int Input::getMousePositionX() {
    return mousePositionX;
}

int Input::getMousePositionY() {
    return mousePositionY;
}

void Input::postMouseChangedPosition(int x, int y) {
    mousePositionX = x;
    mousePositionY = y;
}

void Input::setWindowSize(Size size) {
    windowSize = size;
}

Size Input::getWindowSize() {
    return windowSize;
}

void Input::nextFrame() {
    frame++;
}

} // namespace Panda
