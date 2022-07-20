//
// Created by Admin on 05.02.2022.
//

#include "Input.hpp"

namespace Panda {

uint32_t Input::frame;
bool Input::keys[1024];
uint32_t Input::framesKeys[1024];
bool Input::mouseButtons[8];
uint32_t Input::framesMouseButtons[4];
GSize Input::windowSize;
int Input::mousePositionX;
int Input::mousePositionY;

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

void Input::setWindowSize(GSize size) {
    windowSize = size;
}

GSize Input::getWindowSize() {
    return windowSize;
}

void Input::update() {
    frame++;
}

} // namespace Panda
