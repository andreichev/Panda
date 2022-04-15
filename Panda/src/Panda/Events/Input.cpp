//
// Created by Admin on 05.02.2022.
//

#include "Input.hpp"

namespace Panda {

void Input::setKeyPressed(Key key, bool state) {
    keys[(int)key] = state;
    framesKeys[(int)key] = frame;
}

void Input::setMouseButtonPressed(MouseButton mouseButton, bool state) {
    mouseButtons[(int)mouseButton] = state;
    framesMouseButtons[(int)mouseButton] = frame;
}

bool Input::isKeyPressed(Key key) {
    return keys[(int)key];
}

bool Input::isKeyJustPressed(Key key) {
    return keys[(int)key] && framesKeys[(int)key] == frame - 1;
}

bool Input::isMouseButtonPressed(MouseButton mouseButton) {
    return mouseButtons[(int)mouseButton];
}

bool Input::isMouseButtonJustPressed(MouseButton mouseButton) {
    return mouseButtons[(int)mouseButton] && framesMouseButtons[(int)mouseButton] == frame - 1;
}

int Input::getMousePositionX() {
    return mousePositionX;
}

int Input::getMousePositionY() {
    return mousePositionY;
}

void Input::addWindowSizeListener(WindowSizeListener *listener) {
    windowSizeListeners.push_back(listener);
}

void Input::removeWindowSizeListener(WindowSizeListener *listener) {
    windowSizeListeners.erase(find(windowSizeListeners.begin(), windowSizeListeners.end(), listener));
}

void Input::windowSizeChanged(GSize size) {
    windowSize = size;
    for (auto &listener : windowSizeListeners) {
        listener->windowSizeChanged(size);
    }
}

void Input::postMouseChangedPosition(int x, int y) {
    mousePositionX = x;
    mousePositionY = y;
}

GSize Input::getWindowSize() {
    return windowSize;
}

void Input::eventProcessingFinished() {
    frame++;
}

} // namespace Panda
