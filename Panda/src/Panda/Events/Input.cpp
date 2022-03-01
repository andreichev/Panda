//
// Created by Admin on 05.02.2022.
//

#include "Input.hpp"

namespace Panda {

void Input::setKeyPressed(Key key, bool state) {
    keys[(int) key] = state;
}

bool Input::isKeyPressed(Key key)  {
    return keys[(int) key];
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
    for(auto& listener: windowSizeListeners) {
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

}
