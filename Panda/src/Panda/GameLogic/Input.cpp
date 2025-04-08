//
// Created by Admin on 05.02.2022.
//

#include "Panda/GameLogic/Input.hpp"

#include <Fern/Events/WindowEvents.hpp>
#include <Fern/Events/TouchEvents.hpp>
#include <Fern/Events/KeyEvents.hpp>
#include <Fern/Events/MouseEvents.hpp>
#include <Foundation/Foundation.hpp>
#include <vector>

namespace Panda {

uint32_t Input::frame;
bool Input::keys[1024];
uint32_t Input::framesKeys[1024];
bool Input::mouseButtons[8];
uint32_t Input::framesMouseButtons[4];
Size Input::windowSize;
double Input::mousePositionX = 0;
double Input::mousePositionY = 0;
double Input::mouseDeltaX = 0;
double Input::mouseDeltaY = 0;
double Input::mouseScrollX = 0;
double Input::mouseScrollY = 0;
bool Input::_isTrackpadScroll = false;
Rect Input::viewportFrame;
std::vector<Input::Touch> Input::activeTouches;

void Input::onEvent(Fern::Event *event) {
    using namespace Fern;
    switch (event->type) {
        case EventType::None:
            break;
        case EventType::WindowCloseRequest:
            break;
        case EventType::QuitRequest:
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
        case EventType::InputCharacter: {
            // TODO: Add entered character sequence in Input. Can be used in cheats
            break;
        }
        case EventType::MouseMoved: {
            const MouseMovedEvent *ev = static_cast<const MouseMovedEvent *>(event);
            Input::postMouseChangedPosition(ev->x, ev->y, ev->dx, ev->dy);
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
        case EventType::MouseScrolled: {
            const MouseScrolledEvent *ev = static_cast<const MouseScrolledEvent *>(event);
            Input::postScrollEvent(ev->xoffset, ev->yoffset, ev->isTrackpad);
            break;
        }
        case EventType::TouchBegan: {
            const TouchBeganEvent *ev = static_cast<const TouchBeganEvent *>(event);
            Input::postTouchBeganEvent(ev->id, ev->x, ev->y);
            break;
        }
        case EventType::TouchMoved: {
            const TouchMovedEvent *ev = static_cast<const TouchMovedEvent *>(event);
            Input::postTouchMovedEvent(ev->id, ev->x, ev->y);
            break;
        }
        case EventType::TouchEnded: {
            const TouchEndedEvent *ev = static_cast<const TouchEndedEvent *>(event);
            Input::postTouchEndedEvent(ev->id);
            break;
        }
    }
}

void Input::setKeyPressed(Fern::Key key, bool state) {
    keys[(int)key] = state;
    framesKeys[(int)key] = frame + 1;
}

void Input::setMouseButtonPressed(Fern::MouseButton mouseButton, bool state) {
    mouseButtons[(int)mouseButton] = state;
    framesMouseButtons[(int)mouseButton] = frame + 1;
}

bool Input::isKeyPressed(Fern::Key key) {
    return keys[(int)key];
}

bool Input::isKeyJustPressed(Fern::Key key) {
    return keys[(int)key] && framesKeys[(int)key] == frame;
}

bool Input::isMouseButtonPressed(Fern::MouseButton mouseButton) {
    return mouseButtons[(int)mouseButton];
}

bool Input::isMouseButtonJustPressed(Fern::MouseButton mouseButton) {
    return mouseButtons[(int)mouseButton] && framesMouseButtons[(int)mouseButton] == frame;
}

double Input::getMousePositionX() {
    return mousePositionX;
}

double Input::getMousePositionY() {
    return mousePositionY;
}

double Input::getMouseDeltaX() {
    return mouseDeltaX;
}

double Input::getMouseDeltaY() {
    return mouseDeltaY;
}

double Input::getMouseViewportPositionX() {
    return Foundation::max(mousePositionX - viewportFrame.origin.x, 0.0);
}

double Input::getMouseViewportPositionY() {
    return Foundation::max(mousePositionY - viewportFrame.origin.y, 0.0);
}

double Input::getMouseScrollX() {
    return mouseScrollX;
}

double Input::getMouseScrollY() {
    return mouseScrollY;
}

bool Input::isTrackpadScroll() {
    return _isTrackpadScroll;
}

void Input::postMouseChangedPosition(double x, double y, double dx, double dy) {
    mousePositionX = x;
    mousePositionY = y;
    mouseDeltaX = dx;
    mouseDeltaY = dy;
}

void Input::postScrollEvent(double x, double y, bool isTrackpad) {
    mouseScrollX = x;
    mouseScrollY = y;
    _isTrackpadScroll = isTrackpad;
}

void Input::setWindowSize(Size size) {
    windowSize = size;
}

Size Input::getWindowSize() {
    return windowSize;
}

void Input::nextFrame() {
    frame++;
    mouseScrollX = 0;
    mouseScrollY = 0;
    mouseDeltaX = 0;
    mouseDeltaY = 0;
}

Input::Touch Input::getTouch(int index) {
    if (index >= 0 && index < activeTouches.size()) { return activeTouches[index]; }
    PND_ASSERT_F(false, "WRONG TOUCH INDEX %d", index);
    return Input::Touch(0, 0, 0);
}

int Input::touchCount() {
    return (int)activeTouches.size();
}

void Input::postTouchBeganEvent(int id, float x, float y) {
    activeTouches.emplace_back(id, x, y);
}

void Input::postTouchMovedEvent(int id, float x, float y) {
    auto touch = std::find_if(activeTouches.begin(), activeTouches.end(), [id](auto touch) {
        return touch.id == id;
    });
    // PND_ASSERT(touch != activeTouches.end(), "TOUCH NOT FOUND");
    if (touch != activeTouches.end()) {
        touch->x = x;
        touch->y = y;
    }
}

void Input::postTouchEndedEvent(int id) {
    auto touch = std::find_if(activeTouches.begin(), activeTouches.end(), [id](auto touch) {
        return touch.id == id;
    });
    // PND_ASSERT(touch != activeTouches.end(), "TOUCH NOT FOUND");
    if (touch != activeTouches.end()) { activeTouches.erase(touch); }
}

void Input::setViewportFrame(Rect _frame) {
    viewportFrame = _frame;
}

} // namespace Panda
